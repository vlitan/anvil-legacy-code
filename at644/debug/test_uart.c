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
void	ack_write()
{
	while (uart_getc(LOG_PORT) != 'C');
	uart_puts(LOG_PORT, "rexes\n");
}

int main(void)
{
	int	state = 1;
	uint8_t register i;
	a = b = 0;
	
	
	uart_init(LOG_PORT, BAUD_CALC(115200));
	sei();
	uart_puts(LOG_PORT, "BEGIN\n");
	DDRB = 0xff;
	PORTB = 0xff;
	_delay_ms(100);
	PORTB = 0;
	
	while (1)
	{
		log_motors(a, b);
		if (a > 255)
		{
			a = 0;
			b = -255;
		}
		a++;
		b++;
		PORTB = 0xff;
		_delay_ms(100);
		PORTB = 0;
		_delay_ms(100);
		if (a & 1)
			log_str("I love strings");
		else
			log_str("Extremely");
	}
	return (0);
} 
