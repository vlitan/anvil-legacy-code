/*moves inside the circle*/
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "utils.h"
#include "motors.h"
#include "sensors.h"
#include "usart.h"
#include "logs.h"
#include "start.h"
#include "wd.h"
#include "misc.h"

extern uint8_t	speed_limit;
extern uint8_t	speed_factor;

uint16_t	line_sens;
uint16_t	enemy_sens;
int16_t		speed_a;
int16_t		speed_b;
uint8_t		current_a;
uint8_t		current_b;


void main(void)
{
	uint8_t register i;
	uint8_t state;

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
	speed_factor =  get_speed_factor();
	speed_limit = 150;
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
		if (ON_LINE)
		{
			if (LINE(BL) || LINE(BR))
			{
				speed_a = 120;
				speed_b = 120;
			}
			else if (LINE(FL) || LINE(FR))
			{
				speed_a = -120;
				speed_b = -120;
			}
			third_board_only_one(1);
		}
		else if (ON_ENEMY)
		{
			speed_a = 150;
			speed_b = 150;
			third_board_only_one(2);
		}
		else
		{
			speed_a = 50;
			speed_b = -50;
			third_board_only_one(3);
		}
		go(speed_a, speed_b);
		
		#ifdef LOG
			if (state)
			{
				log_str("Attack!");
			}
			else
			{
				log_str("On_line");
			}
			log_enemy(enemy_sens);
			log_line(line_sens);
		#endif
		wdt_reset();
	}
	return (0);
} 
