#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

#include "utils.h"
#include "motors.h"

int16_t speed_a;
int16_t speed_b;

int main(void)
{
	/*init all the things*/
	motors_init(PHASE_CORRECT, PRESCALE_8);
	/********************/
	go(0, 0);
	speed_a = 255;
	speed_b = -255;
	while (1)
	{
		speed_a--;
		speed_b++;
		if (speed_a == -254)
		{
			speed_a = 255;
			speed_b = -255;
		}
	}
	return (0);
}
