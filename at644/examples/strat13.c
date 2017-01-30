/*moves inside the circle*/
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "utils.h"
#include "motors.h"
#include "sensors.h"
#include "usart.h"
#include "logs.h"
#include "start.h"
#include "wd.h"
#include "misc.h"

#define		SPIN_CONST	125

const int16_t dponds[7] = {-150, -130, -70, 0, 70, 130, 150};
//const int16_t eponds[7] = {5, 30, 60, 100, 60, 30, 5};
const int16_t eponds[7] = {5, 30, 70, 100, 70, 30, 5};

inline void	on_enemy(void);
inline void	on_line(void);
inline void	idle(void);

extern uint8_t	speed_limit;	//stores the abs speed limit.
extern uint8_t	speed_factor;	//stores the speed factor
uint8_t			no_enemy;		//stores the beh. when no enemy.

uint16_t	line_sens;
uint16_t	enemy_sens;
uint8_t 	state;
int16_t		speed_a;
int16_t		speed_b;

int16_t		spin;
int16_t		speed_base;
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
	/**************************/
	
	/*read and set params*/
	no_enemy = SPIN;//get_no_enemy();
	speed_factor = get_speed_factor();
	speed_limit = 250;
	speed_base = 150;
	speed_eq = 0;
	spin = 0;
	/*********************/
	#ifdef LOG
		log_str("BEGIN!");
	#endif
	go(0, 0);
	
	third_board_set(1, 1);
	third_board_set(2, 1);
	third_board_set(3, 1);
	
	wait_on_start();
	#ifdef LOG
		log_str("GO!");
	#endif
	
	third_board_set(1, 0);
	third_board_set(2, 0);
	third_board_set(3, 0);
	WDT_Init();

	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		
		if (ON_ENEMY && ON_LINE)
		{
			on_line();
			state = 0;
		}
		else if (ON_ENEMY)
		{
			on_enemy();
			state = 1;
		}
		else if (ON_LINE)
		{
			on_line();
			state = 2;
		}
		else
		{
			idle();
			state = 3;
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
	

	if ((ENEMY(0) && ENEMY(7)) || (ENEMY(1) && ENEMY(6)))
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
			spin += dponds[i];
			speed_eq += eponds[i];
			++view_count;
		}
	}
	spin /= view_count;
	speed_eq /= view_count;
	speed_a = speed_base + speed_eq + spin;
	speed_b = speed_base + speed_eq - spin;
}

inline void	on_line(void)
{
/*	if (LINE(FR) && LINE(FL))
	{
		speed_a = speed_b = -250;
		go(speed_a, speed_b);
		_delay_ms(10);
	}*/
	if (LINE(BL) && LINE(BR))
	{
		speed_a = speed_b = 250;
	}
	if (LINE(FL) && LINE(FR))
	{
		speed_a = speed_b = -250;
	}
	
	if (LINE(FL) || LINE(BL))
	{
		spin = -SPIN_CONST;
	}
	else if (LINE(BR) || LINE(FR))
	{
		spin = SPIN_CONST;
	}
	
	if ((LINE(BL) && LINE(FR)) || (LINE(FL) && LINE(BR)))
	{
		idle();
		state = 3;
		return;	
	}
	speed_a = speed_base + spin;
	speed_b = speed_base - spin;
	go(speed_a, speed_b);
	_delay_ms(15);
}

inline void idle(void)
{
/*	if (no_enemy == SPIN)
	{
		speed_a = SPIN_CONST;
		speed_b = -SPIN_CONST;
	}*/
	if (no_enemy == SPIN)
	{
		if (speed_a >= speed_b)
		{
			spin = SPIN_CONST; 
		}
		else if (speed_a < speed_b)
		{
			spin = -SPIN_CONST;
		}
	}
	speed_a = spin;
	speed_b = -spin;

//	else if (no_enemy == SEEK)
//	{
//		speed_a /= 2;
//		speed_b /= 2;
//	}
//	speed_a = speed_b = 0;
}
