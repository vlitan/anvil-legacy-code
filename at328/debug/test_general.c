#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "utils.h"
#include "motors.h"
#include "usart.h"
#include "communication.h"
#include "analog.h"
#include "wd.h"

int16_t a, b;
uint8_t	req_t;

int main(void)
{
	motors_init(PHASE_CORRECT, PRESCALE_8);
	uart_init(BAUD_CALC(9600));
	sei();
	adc_init();
	wdt_init(WDTO_4S);
	
	uart_puts("BEGIN\n");
	go(0, 0);
	uint8_t dbg = 0;
	uart_putc(ACK);
	set_reg_pin(&DDRD, 2, 1);

	set_reg_pin(&PORTD, 2, 1);
	_delay_ms(500);
	set_reg_pin(&PORTD, 2, 0);
	_delay_ms(500);
	set_reg_pin(&PORTD, 2, 1);
	_delay_ms(500);
	set_reg_pin(&PORTD, 2, 0);
	while (1)
	{
		req_t = get_request_type();
		if (req_t != NONE) wdt_reset();
		if (req_t == CURRENT_REQ)
		{	
			write_current(analog_read(C_SENS_A), analog_read(C_SENS_B));
		}
		else if (req_t == MOTORS_SET)
		{
			read_motors(&a, &b);
		}
		//manage current and speed here
		go(a, b);

	}
	return (0);
}
