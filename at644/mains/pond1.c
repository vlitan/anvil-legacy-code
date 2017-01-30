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
extern float	speed_factor;

uint16_t	line_sens;
uint16_t	enemy_sens;
int16_t		speed_a;
int16_t		speed_b;
uint8_t		current_a;
uint8_t		current_b;
uint8_t		state;

void main(void)
{
	uint8_t register i;

	/*init all the things!*/
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(9600));
	#endif
	uart_init(COMMAND_PORT, BAUD_CALC(9600));
	third_board_init();
	sei();
	start_init();
	sensors_init();
	r328_init();
	/**************************/
	
	/*read and set params*/
	speed_limit = 120;
	speed_factor = 2;
	/*********************/

	#ifdef LOG
		log_str("BEGIN!");
	#endif
	go(0, 0);
	wait_on_start();
	#ifdef LOG
		log_str("GO!");
	#endif
	reset_328();
	WDT_Init();
	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		if (ON_ENEMY)
		{
			state = 1;
			if (ON_LINE)
			{
				state = 3;
			}
			
		}
		else if (ON_LINE)
		{
			state = 2;
		}
		else
		{
			state = 0;
		}
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
			read_current(&current_a, &current_b);
			log_current(current_a, current_b);
			log_enemy(enemy_sens);
			log_line(line_sens);
		#endif
		wdt_reset();
	}
	return (0);
} 
