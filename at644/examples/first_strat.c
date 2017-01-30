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

#define		SPIN_CONST	30

PROGMEM const int16_t ponds[7] = {90, 60, 45, 0, -45, -60, -90};

inline void	on_enemy(void);
inline void	on_line(void);
inline void	idle(void);

extern uint8_t	speed_limit;	//stores the abs speed limit.
extern uint8_t	speed_factor;	//stores the speed factor
uint8_t			no_enemy;		//stores the beh. when no enemy.

uint16_t	line_sens;
uint16_t	enemy_sens;
int16_t		speed_a;
int16_t		speed_b;
int16_t		speed_const;
uint8_t 	state;
int16_t		spin;

void main(void)
{
	uint8_t register i;

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
	speed_factor =  get_speed_factor();
	speed_limit = 255;
	speed_const = 150;
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
	WDT_Init();

	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);

		if (ON_ENEMY && ON_LINE)
		{
			on_enemy();
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
	uint8_t register	i;
	uint8_t				view_count;
	
	spin = 0;
	view_count = 0;
	for (i = 0; i < ENEMY_COUNT; ++i)
	{
		if (ENEMY(i))
		{
			spin += ponds[i];
			++view_count;
		}
	}
	spin /= view_count;
	speed_a = speed_const + spin;
	speed_b = speed_const - spin;
}

inline void	on_line(void)
{
	if (LINE(FL))
	{
		spin = SPIN_CONST;
	}
	else if (LINE(FR))
	{
		spin = -SPIN_CONST;
	}
	
	if (LINE(BL))
	{
		spin += SPIN_CONST;
	}
	else if (LINE(BR))
	{
		spin += -SPIN_CONST;
	}
	if ((LINE(BL) && LINE(FR)) || (LINE(FL) && LINE(BR)))
	{
		speed_a = 0;
		speed_b = 0;
		return;	
	}
	speed_a = speed_const + spin;
	speed_b = speed_const - spin;
}

inline void idle(void)
{
	if (no_enemy == SPIN)
	{
		speed_a = SPIN_CONST;
		speed_b = -SPIN_CONST;
	}
//	else if (no_enemy == SEEK)
//	keep current values;
}
