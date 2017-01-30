#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "utils.h"
#include "motors.h"
#include "usart.h"

void blink(void)
{
	PORTC = 0xff;
	_delay_ms(100);
	PORTC = 0;
	_delay_ms(100);
}

void toggle(uint8_t pin)
{
	toggle_reg_pin(&PORT(C), pin);
}

void main(void)
{
	DDRC = 0xff;
	set_motors(PHASE_CORRECT, PRESCALE_8);
	uart_init(BAUD_CALC(115200));
	sei();
	uint16_t a, b;
	PORTC = 0xff;
	go(0, 0);
	a = 0;
	b = 0;
	while (1)
	{
		read_motors(&a, &b);
		go(a, b);
	}
}
