#include "global.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
//echoes the input with +1
int main(void)
{
	char c;
	uart_init(LOG_PORT, BAUD_CALC(9600));
	sei();
	uart_puts(LOG_PORT, "cm\n");
	DDRB = 0xff;
	PORTB = 0;
	while (1)
	{
		c = uart_getc(LOG_PORT);
		if (c != 0)
		{
			uart_putc(LOG_PORT, c);
			//uart_putc(c);
			if (c < 0)
				PORTB |= 1;
			else if (c > 0)
				PORTB |= 2;
			/*
			else if (c == 'b')
				PORTB = 0;
			if (c == 'c')
				uart_puts("VICCCCCCCA\n");
			else if (c == 'm')
				uart_puts("sudo\n");
			else
				uart_puts("nono\n");*/
		}
	}
	return (0);
}
