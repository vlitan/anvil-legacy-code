#define F_CPU 20000000UL

#include <avr/io.h>
#include "pwm.h"
#include <util/delay.h>



int main(void)
{
	int i = 0;
	DDRD = 0xff;
	DDRB = 0xff;
	set_timer(0, PHASE_CORRECT, PRESCALE_8);
	set_timer(2, PHASE_CORRECT, PRESCALE_8);
	while (1)
	{
		i++;
		set_match_value(0, B, i);
		if (i > 256)
			i = 0;
	}
}
