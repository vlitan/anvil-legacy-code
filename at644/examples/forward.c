/*goes back and forward, stops at line*/
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
	speed_factor = 1;//get_speed_factor();
	speed_limit = 240;
	/*********************/

	#ifdef LOG
		log_str("BEGIN!");
	#endif
	go(0, 0);
	third_board_all(1);
	wait_on_start();
	#ifdef LOG
		log_str("GO!");
	#endif
	third_board_all(0);
//	speed_a = speed_b = 50;
	WDT_Init();
	while (1)
	{
		read_line_sens(&line_sens);
		if (ON_LINE)
		{
			if (LINE(BL) || LINE(BR))
			{
				speed_a = 200;
				speed_b = 200;
			}
			else if (LINE(FR) || LINE(FL))
			{
				speed_a = -200;
				speed_b = -200;
			}
		}
	/*	else
		{
			speed_a = speed_b = 0;
		}*/
		go(speed_a, speed_b);
		
		#ifdef LOG
			if (uart_getc(COMMAND_PORT) == '!')
				log_str("CURRENT");
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
