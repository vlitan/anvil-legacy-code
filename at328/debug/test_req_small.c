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

int16_t a, b;
uint8_t	req_t;

int main(void)
{
	motors_init(PHASE_CORRECT, PRESCALE_8);
	uart_init(BAUD_CALC(115200));
	adc_init();
	sei();
	go(0, 0);
//	uart_putc(ACK);
	DDRB = 0xff;
	set_reg_pin(&PORTB, 1, 1);
	_delay_ms(100);
	toggle_reg_pin(&PORTB, 1);
	_delay_ms(100);
	while (1)
	{
		req_t = get_request_type();
		if (req_t == CURRENT_REQ)
		{
			set_reg_pin(&PORTB, 2, 1);
			set_reg_pin(&PORTB, 1, 0);
			uart_putc('C');
			uart_putc('\n');
	//		go_0(0);
	//		go_2(10);
	//		write_current(analog_read(MOTOR_A), analog_read(MOTOR_B));
		}
		else if (req_t == MOTORS_SET)
		{
			set_reg_pin(&PORTB, 1, 1);
			set_reg_pin(&PORTB, 2, 0);
			uart_putc('M');
			uart_putc('\n');
	//		go_0(10);
	//		go_2(0);
	//		read_motors(&a, &b);
		}
	}
	return (0);
}
