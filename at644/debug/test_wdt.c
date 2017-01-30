#define F_CPU 20000000UL
#include <avr/io.h>
#include "wd.h"
#include "start.h"
#include <util/delay.h>

//Prepare LED pin
void PortInit(void)
{
    //PD2 as output
    DDRB|=(1<<PB1);
    //Initlally LED ON
    PORTB|=(1<<PB1);
}

int main(void)
{
	
    //Initialize port
    PortInit();
    //initialize watchdog in reset mode
	WDT_Init();
    //delay to detet reset
    _delay_ms(500);
    while(1)
    {
        //LED ON
    PORTB|=(1<<PB1);
    _delay_ms(20);
        //LED OFF
    PORTB&=~(1<<PB1);
        //~0.5s delay
    _delay_ms(500);
//	wdt_reset();
    }
	return (0);
}

