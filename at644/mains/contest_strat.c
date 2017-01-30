/*moves inside the circle*/
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <util/atomic.h>

#include "defines.h"
#include "utils.h"
#include "motors.h"
#include "sensors.h"
#include "usart.h"
#include "logs.h"
#include "start.h"
#include "wd.h"
#include "misc.h"

//configure those values
#define	SPIN_FACTOR			200
#define	FORWARD_FACTOR		240
#define FORWARD_DEFAULT		0
#define	SPIN_CONST			170

//set according to sensors angles
								// -90    -60    -30      0  30     60     90
double	fsinf[7] = {-1,   -0.866, -0.5,   0, 0.5,   0.866, 1};
double	fcosf[7] = {0.1 ,  0.5  ,  0.86, 1.1, 0.86, 0.5  , 0.1};

//weights for spinning and forwarding
int16_t spin_weights[7];
int16_t for_weights[7];
//int16_t spin_weights[7] = {-150, -130, -70, 0, 70, 130, 150};
//int16_t for_weights[7] = {5, 30, 70, 100, 70, 30, 5};

void		compute_weights(void);
void		control_state(void);
inline void	on_enemy(void);
inline void	on_line(void);
inline void	idle(void);

extern uint8_t	speed_limit;	//stores the abs speed limit.
extern uint8_t	speed_factor;	//stores the speed factor
uint8_t		no_enemy;		//stores the beh. when no enemy.
uint8_t		default_dir;	//stores the default spin

//sensors vars
uint16_t	line_sens;
uint16_t	enemy_sens;
//state and speed
uint8_t 	state;
int16_t		speed_right;
int16_t		speed_left;
//speed compute varialbles. could and should be local.
int16_t		spin;
int16_t		speed_eq;
//go(DREAPTA, STANGA);

volatile uint8_t	changed_line_value;
unsigned long		spin_time_ms;
unsigned long		get_time_ms();
void				timer0_init();
volatile unsigned long	ms;
void main(void)
{
	/*init all the things!*/
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(9600));
	#endif
	uart_init(COMMAND_PORT, BAUD_CALC(57600));
	third_board_init();
	start_init();
	sensors_init();
	timer0_init();
	sei();
	compute_weights();
	/**************************/

	/*read and set params*/
	no_enemy = get_no_enemy();//NEW CHECK
	speed_factor = get_speed_factor();
	default_dir = get_default_dir();//for spin only `	NEW CHECK
	speed_limit = BLANA - 5;
	/*********************/
	if (get_control())
	{
		control_state();
	}
	#ifdef LOG
		log_str("BEGIN!");
	#endif
	go(0, 0);			//make sure the robot is still
	third_board_all(1);	//NEW
	wait_on_start(get_start_mode());	//waits the start module
	#ifdef LOG
		log_str("GO!");
	#endif
	third_board_all(0);	//NEW
	WDT_Init();			//start watchdog

	enemy_sens = 0x7f;
	spin_time_ms = 0;
	speed_left = speed_right = 0;
	read_line_sens(&line_sens);		//read line sensors
	while (1)
	{
		if (changed_line_value == 1) //if line value changed
		{
			read_line_sens(&line_sens);		//read line sensors
			changed_line_value = 0;
		}
		//read_enemy_sens(&enemy_sens);	//read enemy sensors

		if (ON_ENEMY && ON_LINE)
		{
			on_line();//line beh. is prioritary
			state = ENEMY_AND_LINE_S;
			spin_time_ms = 0;
		}
		else if (ON_ENEMY)
		{
			on_enemy();
			state = ON_ENEMY_S;
			spin_time_ms = 0;
		}
		else if (ON_LINE)
		{
			on_line();
			state = ON_LINE_S;
		}
		else
		{
			idle();
			state = ON_NOTHING_S;
		}
		//write to motors
		go(speed_right, speed_left);
		//display current state
		display_state(state);
//	//display current no_enemy state
//	third_board_set(1, no_enemy == SEEK);

		//reset watchdog
		wdt_reset();

		#ifdef LOG
			log_enemy(enemy_sens);
			log_line(line_sens);
		#endif
	}
}

inline void	on_enemy(void)
{
	uint8_t i;
	uint8_t	view_count;

	if ((ENEMY(0) && ENEMY(7)) || (ENEMY(1) && ENEMY(6)))//obviously an error
	{
		idle();
		state = ON_NOTHING_S;
		return ;
	}
	spin = 0;
	speed_eq = 0;
	view_count = 0;
	for (i = 0; i < ENEMY_COUNT; ++i)
	{
		if (ENEMY(i))
		{
			spin += spin_weights[i];
			speed_eq += for_weights[i];
			++view_count;
		}
	}

	spin /= view_count;
	speed_eq /= view_count;
	speed_right = FORWARD_DEFAULT + speed_eq + spin;
	speed_left = FORWARD_DEFAULT + speed_eq - spin;

	no_enemy = SPIN;//STOP_SEEK
}

//TODO NEW CHECK. this is in the tree
inline void	on_line(void)
{
	//back / front on the line. go back straight.
	if (LINE(BL) && LINE(BR))
	{
		speed_eq = BLANA / 4;
		spin_time_ms = get_time_ms() + 100;
		no_enemy = SEEK;
		spin = 0;
	}
	else if (LINE(FL) && LINE(FR))
	{
		speed_eq = -BLANA;
	//	spin_time_ms = ms + 60;
		no_enemy = SPIN;
		spin = 0;
	}
	//diagonally opposite sensors on the line. go back
	else if ((LINE(BL) && LINE(FR)) || (LINE(FL) && LINE(BR)))
	{
		speed_eq = -BLANA;
		spin = 0;
	//da-i pace
		//no_enemy = SPIN;
	}
	else //sides on the line. spin in place
	{
		speed_eq = 0;
	 	if (LINE(FL) || LINE(BL))
		{
			spin = -(SPIN_CONST + 50);
		}
		else if  (LINE(BR) || LINE(FR))
		{
			spin = SPIN_CONST + 50;
		}
/////////check and rethink this
		if (spin_time_ms > ms)
		{
			return;
		}
///////////////////////////////
		if (LINE(BR) || LINE(BL))
		{
			speed_eq = 60;
		}
		if (LINE(FR) || LINE(FL))
		{
			speed_eq = -60;
		}
	  	spin_time_ms = get_time_ms() + 80;
		no_enemy = SEEK;
	}

	speed_right = speed_eq + spin;
	speed_left = speed_eq - spin;
}

inline void idle(void)
{
	if (spin_time_ms > get_time_ms())
	{
		return;
	}
//	speed_right = 120;
//	speed_left = 120;
//	return;
//	third_board_set(1, 0);
	spin_time_ms = 0;
	spin = 0;
	speed_eq = 0;
	if (no_enemy == SPIN)
	{
		if (speed_right > speed_left)
		{
			spin = SPIN_CONST;
		}
		else if (speed_right < speed_left)
		{
			spin = -SPIN_CONST;
		}
		else //if ((speed_right < 10) && (speed_left < 10))
		{
			spin = (default_dir == RIGHT) ? -SPIN_CONST:SPIN_CONST;//NEW TODO CHECK
		}
	//	else
	//	{
	//		no_enemy = SEEK;
	//	}
	}
	else //if (no_enemy == SEEK)
	{
		//third_board_set(1, 1);
		speed_eq = BLANA / 2;
	}
	speed_right = speed_eq + spin;
	speed_left = speed_eq - spin;
}


void	control_state(void)
{
	uint8_t opt;

	swipe_leds(RIGHT_LEFT, 2);
	set_reg_pin(&PCICR, START_PCIE, 0);
	set_reg_pin(&START_PCMSK, START_PCINT, 0);
	while(get_control())
	{
		read_line_sens(&line_sens);		//read line sensors
		read_enemy_sens(&enemy_sens);	//read enemy sensors
		switch (get_control_query())
		{
			case DISPLAY_ENEMY1 :
					display_number((ENEMY(0)) | (ENEMY(1) << 1) | (ENEMY(2) << 2));
				break;
			case DISPLAY_ENEMY2 :
					display_number((ENEMY(3)) | (ENEMY(4) << 1) | (ENEMY(5) << 2));
				break;
			case DISPLAY_ENEMY3 :
					display_number(ENEMY(6));
				break;
			case DISPLAY_LINE1 :
					display_number((LINE(0)) | (LINE(1) << 1) | (LINE(2) << 2));
				break;
			case DISPLAY_LINE2 :
					display_number(LINE(3));
				break;
			case DISPLAY_START :
					display_number(get_bit(PIN(START_REG), START_PIN));
				break;
			case NOT_AN_OPTION:
					swipe_leds(LEFT_RIGHT, 1);
					swipe_leds(RIGHT_LEFT, 1);
				break;
		}

	}
	set_reg_pin(&PCICR, START_PCIE, 1);
	set_reg_pin(&START_PCMSK, START_PCINT, 1);
	swipe_leds(LEFT_RIGHT, 2);
}

//uses global variables. do not move from main
void	compute_weights(void)
{
	int i;

	for (i = 0; i < 7; ++i)
	{
		spin_weights[i] = (int)round(fsinf[i] * SPIN_FACTOR);
		for_weights[i] = (int)round(fcosf[i] * FORWARD_FACTOR);
	}
}

ISR (PCINT2_vect)
{
	changed_line_value = 1;
}

ISR (TIMER1_COMPA_vect)
{
	++ms;
}

unsigned long last_ms;
unsigned long get_time_ms()
{

	if (last_ms > ms)
	{
		spin_time_ms = 0;
	}
	last_ms = ms;
//	ATOMIC_BLOCK(ATOMIC_FORCEON)
//	{
//		millis_return = ms;
//	}
	return (ms);
}

void timer0_init()
{
	last_ms = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
	OCR1AL = (CTC_MATCH_OVERFLOW & 0xff);
	TIMSK1 |= (1 << OCIE1A);
}
