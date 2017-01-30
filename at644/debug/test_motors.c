#define F_CPU 20000000UL
#include <util/delay.h>

#include "global.h"
#include "motors.h"
#include "usart.h"
#include <avr/interrupt.h>
#define DELAYF 70

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

void	test_motors1(int i, int speed)
{
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

void test_motors2(void)
{
	go(200, 200);
	_delay_ms(50);
	go(-200, -200);
	_delay_ms(50);
	go(100, 100);
	_delay_ms(100);
	go(-123, 123);
	_delay_ms(40);
	go(123, -123);
	_delay_ms(40);
}

void test_motors3(int16_t speed)
{
	go(speed, speed);
	_delay_ms(100);
	go(-speed, -speed);
	_delay_ms(100);
}

#define MAXSPEED (100)
#define MINSPEED (-MAXSPEED)
int main(void)
{
	//BA
	//set_motors(PHASE_CORRECT, PRESCALE_8);
	uart_init(COMMAND_PORT, BAUD_CALC(57600));
	sei();
	int16_t speed = 30;
	int8_t crement = 1;
	while (1)
	{
		if ((speed >= MAXSPEED) || (speed <= MINSPEED))
		{
			crement *= -1;
		}
		speed += crement;
		go(speed, speed);
		_delay_ms(5);
	}
}
