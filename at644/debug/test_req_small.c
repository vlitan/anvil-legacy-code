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

extern uint8_t	speed_limit;
extern float	speed_factor;

uint16_t	line_sens;
uint16_t	enemy_sens;
int16_t		a;
int16_t		b;
uint8_t		ca;
uint8_t		cb;
int			alert;

int main(void)
{
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(115200));
	#endif
	#ifdef SEC_MCU
		uart_init(COMMAND_PORT, BAUD_CALC(115200));
	#endif
	#if defined(LOG) || defined(SEC_MCU)
		sei();
	#endif
	sensors_init();
	#ifndef SEC_MCU
		motors_init(PHASE_CORRECT, PRESCALE_8);
	#endif
	speed_limit = 249;
	speed_factor = 1;
//	log_motors(1, 1);
//	go(0, 0);
	uint8_t register i;
	int	state = 0;
	a = b = 0;
	DDRB = 0xff;
	PORTB = 0xff;
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
		{
			uart_putc(COMMAND_PORT, 'c');
			set_reg_pin(&PORTB, 0, 1);
			set_reg_pin(&PORTB, 1, 0);
			while (uart_getc(COMMAND_PORT) != 'C');
			toggle_reg_pin(&PORTB, 0);
			
		}
		else
		{
			uart_putc(COMMAND_PORT, 'm');
			set_reg_pin(&PORTB, 1, 1);
			set_reg_pin(&PORTB, 0, 0);
			while (uart_getc(COMMAND_PORT) != 'M');
			toggle_reg_pin(&PORTB, 1);
		}
	}
	return (0);
} 
