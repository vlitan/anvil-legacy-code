#ifndef F_CPU 
	#define F_CPU 16000000UL
#endif
# include <avr/interrupt.h>
# include "communication.h"
# include "usart.h"
# include "motors.h"

/**/
void	write_current(uint8_t ca, uint8_t cb)
{
	char s[5];
	
	s[0] = 'c';
	s[1] = (ca == 255) ? 255 : (ca + 1);
	s[2] = (cb == 255) ? 255 : (cb + 1);
	s[3] = 0;
	uart_puts(s);
//	while (uart_getc() != ACK); //wait for ack byte
}

/*****************
reads the value for motors
******************/
void		read_motors(int16_t *a, int16_t *b)
{
	uint8_t s[5];

	uart_getln(s, 5);
	*a = s[1];		//default case
	if (s[0] == 2)
	{
		*a = -(*a);
	}
	else if (s[0] == 3) //else if is mandatory!
	{
		*a = 0;
	}
	*b = s[3];
	if (s[2] == 2)
		*b = -(*b);
	else if (s[2] == 3)
		*b = 0;
	//ack
	s[0] = ACK;
	s[1] = 0;
	uart_puts(s);
}

uint8_t	get_request_type(void)
{
	char req;

	req = uart_getc();
	if (req == 'c')
	{
		return (CURRENT_REQ);
	}
	else if (req == 'm')
	{
		return (MOTORS_SET);
	}
	else
		return (NONE);
}

inline void	write_alert(uint8_t alert_code)
{
	uart_putc('a');
	uart_putc(alert_code);
	uart_putc('\n');
}
