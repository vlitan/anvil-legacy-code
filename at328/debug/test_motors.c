#define F_CPU 16000000UL
#include <util/delay.h>

#include "global.h"
#include "motors.h"
#define DELAYF 50

/********************
due to a crazy-ass error from _delay_ms
(occurs when trying to call it with a variable in go_the_display)
the DELAYF define and other nasty tricks are needed
*********************/

void	go_then_delayf(int speed0, int speed2)
{
	go(speed0, speed2);
	_delay_ms(DELAYF);
}

void	test_motors(int i, int speed)
{
	set_motors(PHASE_CORRECT, PRESCALE_8);
	while (i--)
	{
		go_then_delayf(0, speed);
		go_then_delayf(0, -speed);
		go_then_delayf(speed, 0);
		go_then_delayf(-speed, 0);
		go_then_delayf(speed, speed);
		go_then_delayf(-speed, -speed);
		go_then_delayf(speed, -speed);
		go_then_delayf(-speed, speed);
		go_then_delayf(0, 0);
	}
}

int main(void)
{
	test_motors(5, 123);
}
