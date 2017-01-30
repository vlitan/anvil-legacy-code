#define F_CPU 16000000UL

#include <avr/io.h>
#include "pwm.h"
#include <util/delay.h>



int main(void)
{
	int i = 0;
	DDRD = 0xff;
	DDRB = 0xff;
	set_timer(0, PHASE_CORRECT, PRESCALE_256);
	set_timer(2, PHASE_CORRECT, PRESCALE_8);
	PORTB = 0;
	PORTD &= ~(1 << 5); 
	set_match_value(0, A, 127);
	set_match_value(0, B, 200);
	/*while (1)
	{
		_delay_ms(10);
		i++;
		set_match_value(2, B, i);
	}*/
}
