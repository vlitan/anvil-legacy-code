#define F_CPU 20000000UL

#include <util/delay.h>
#include <avr/io.h>
#include "utils.h"
#include "start.h"
#define LED1	0
#define LED2	1

int main(void)
{

	//outputs
	set_reg_pin(&DDRB, LED1, 1);
	set_reg_pin(&DDRB, LED2, 1);
	set_reg_pin(&PORTB, LED2, 1);
	set_reg_pin(&PORTB, LED1, 1);
	
	//piruete incipient-demonstrative
	_delay_ms(200);
	toggle_reg_pin(&PORTB, LED1);
	_delay_ms(200);
	toggle_reg_pin(&PORTB, LED1);
	_delay_ms(200);
	toggle_reg_pin(&PORTB, LED2);
	init_start();	
	sei();
	
	wait_on_start();
	while (1)
	{
		toggle_reg_pin(&PORTB, LED1);
		_delay_ms(100);
	}	
	return (0);
}

