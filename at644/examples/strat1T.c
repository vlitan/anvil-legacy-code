/*moves inside the circle*/
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>

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
#define	SPIN_FACTOR		150
#define	FORWARD_FACTOR	100
#define FORWARD_DEFAULT	150
#define	SPIN_ONLY		150
//set according to sensors angles
double	sinf[7] = {};
double	cosf[7] = {};

//weights for spinning and forwarding
int16_t spin_weights[7];
int16_t for_weights[7];

//int16_t spin_weights[7] = {-150, -130, -70, 0, 70, 130, 150};
//int16_t for_weights[7] = {5, 30, 70, 100, 70, 30, 5};

void		compute_weights(void);
void		control_state(void)
inline void	on_enemy(void);
inline void	on_line(void);
inline void	idle(void);

extern uint8_t	speed_limit;	//stores the abs speed limit.
extern uint8_t	speed_factor;	//stores the speed factor
uint8_t			no_enemy;		//stores the beh. when no enemy.
uint8_t			default_dir;	//stores the default spin

//sensors vars
uint16_t	line_sens;
uint16_t	enemy_sens;
//state and speed
uint8_t 	state;
int16_t		speed_a;
int16_t		speed_b;
//speed compute varialbles. could and should be local.
int16_t		spin;
int16_t		speed_eq;


void main(void)
{
	/*init all the things!*/
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(9600));
	#endif
	uart_init(COMMAND_PORT, BAUD_CALC(57600));
	third_board_init();
	sei();
	start_init();
	sensors_init();
	compute_weights();
	/**************************/
	
	/*read and set params*/
	no_enemy = get_no_enemy();//NEW CHECK
	speed_factor = get_speed_factor();
	default_dir = get_default_dir();//NEW CHECK
	speed_limit = BLANA;
	/*********************/
	if (get_control())
	{
		control_state();
	}
	#ifdef LOG
		log_str("BEGIN!");
	#endif
	go(0, 0);			//make sure the robot is still
	third_board_set(1);	//NEW
	wait_on_start(get_start_mode());	//waits the start module
	#ifdef LOG
		log_str("GO!");
	#endif
	third_board_set(0);	//NEW
	WDT_Init();			//start watchdog

	while (1)
	{
		read_line_sens(&line_sens);		//read line sensors
		read_enemy_sens(&enemy_sens);	//read enemy sensors
		
		if (ON_ENEMY && ON_LINE)
		{
			on_line();//line beh. is prioritary
			state = ENEMY_AND_LINE;
		}
		else if (ON_ENEMY)
		{
			on_enemy();
			state = ON_ENEMY;
		}
		else if (ON_LINE)
		{
			on_line();
			state = ON_LINE;
		}
		else
		{
			idle();
			state = ON_NOTHING;
		}

		go(speed_a, speed_b);
		display_state(state);
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
		state = 3;
		return ;
	}
	spin = 0;
	view_count = 0;
	speed_eq = 0;
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
	speed_a = FORWARD_DEFAULT + speed_eq + spin;
	speed_b = FORWARD_DEFAULT + speed_eq - spin;
}

//TODO NEW CHECK this is in the tree
inline void	on_line(void)
{
	//back / front on the line. go back straight.
	if (LINE(BL) && LINE(BR))
	{
		speed_eq = BLANA;
		spin = 0;
	}
	else if (LINE(FL) && LINE(FR))
	{
		speed_eq = -BLANA;
		spin = 0;
	}
	//sides on the line. spin in place
	if (LINE(FL) || LINE(BL))
	{
		spin = -SPIN_CONST;
		speed_eq = 0;
	}
	else if (LINE(BR) || LINE(FR))
	{
		spin = SPIN_CONST;
		speed_eq = 0;
	}
	//diagonally opposite sensors on the line. considered error and ignored
	if ((LINE(BL) && LINE(FR)) || (LINE(FL) && LINE(BR)))
	{
		idle();
		state = ON_NOTHING;
		return;
	}
	speed_a = speed_eq + spin;
	speed_b = speed_eq - spin;
	go(speed_a, speed_b);
}

inline void idle(void)
{
	if (no_enemy == SPIN)
	{
		speed_eq = 0;
		if (speed_a > speed_b)
		{
			spin = SPIN_CONST;
		}
		else if (speed_a < speed_b)
		{
			spin = -SPIN_CONST;
		}
		else
		{
			spin = (default_dir == RIGHT) ? -SPIN_CONST:SPIN_CONST;//NEW TODO CHECK
		}
	}
	//this runs only once. before finding the enemy.
	else if (no_enemy == SEEK)
	{
/*>>>>*/no_enemy = SPIN;//!!!<<<<<<<
		spin = 0;
		speed_eq = (default_DIR == FORWARD) ? BLANA : (-BLANA);
	}
	speed_a = speed_eq + spin;
	speed_b = speed_eq - spin;
}


void	control_state(void)
{
	uint8_t opt;

	swipe_leds(LEFT_RIGHT, 3);
	while(get_control())
	{
		read_line_sens(&line_sens);		//read line sensors
		read_enemy_sens(&enemy_sens);	//read enemy sensors
		switch (get_control_query())
		{
			case DISPLAY_ENEMY1 :
					display_number((ENEMY(0)) & (ENEMY(1) << 1) & (ENEMY(2) << 2));
				break;
			case DISPLAY_ENEMY2 : 
					display_number((ENEMY(3)) & (ENEMY(4) << 1) & (ENEMY(5) << 2));
				break;
			case DISPLAY_ENEMY3 :
					display_number(ENEMY(6));
				break;
			case DISPLAY_LINE1 : 
					display_number((LINE(0)) & (LINE(1) << 1) & (LINE(2) << 2));
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
	swipe_leds(RIGHT_LEFT, 3);
}

//uses global variables. do not move from main
void	compute_weights(void)
{
	for (int i = 0; i < 7; ++i)
	{
		spin_weights[i] = (int)round(sinf[i] * SPIN_FACTOR);
		for_weights[i] = (int)round(cosf[i] * FORWARD_FACTOR);
	}
}