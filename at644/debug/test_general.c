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

extern uint8_t	speed_limit;
extern float	speed_factor;

uint16_t	line_sens;
uint16_t	enemy_sens;
int16_t		a;
int16_t		b;
uint8_t		ca;
uint8_t		cb;
int			alert;

void main(void)
{
	uint8_t register i;
	int	state = 1;
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(9600));
	#endif
	#ifdef SEC_MCU
		uart_init(COMMAND_PORT, BAUD_CALC(9600));
	#endif
	sei();
	sensors_init();
	#ifndef SEC_MCU
		motors_init(PHASE_CORRECT, PRESCALE_8);
	#endif
	start_init();

	DDRB = 0xff;
	speed_limit = 230;
	speed_factor = 1;
	go(0, 0);
	log_str("BEGIN");
	wait_on_start();
	WDT_Init();
	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		for (i = 0; i < 9; ++i)
			if (!get_bit(enemy_sens, i))
				state = 1;
		for (i = 0; i < 7; ++i)
			if (!get_bit(line_sens, i))
				state = 0;
		if (state)
			go(a, b);
		else
			go(0, 0);
		if (a % 20 == 0)
		{
			read_current(&ca, &cb);
			log_current(ca, cb);
		}
		log_enemy(enemy_sens);
		log_line(line_sens);
		if (a >= 255)
		{
			log_str("LIMIT!");
			a = 0;
			b = -255;
		}
		if (state)
		{
			a++;
			b++;
		}
	wdt_reset();
	//	_delay_ms(10);
	}
	return (0);
} 
