#define F_CPU 16000000UL
#include <avr/io.h>
#include "wd.h"
#include <util/delay.h>

//Prepare LED pin
void PortInit(void)
{
    //PD2 as output
    DDRD|=(1<<PD2);
    //Initlally LED ON
    PORTD|=(1<<PD2);
}

int main (void)
{
    //Initialize port
    PortInit();
    //initialize watchdog in reset mode
	wdt_init(WDTO_4S);
    //delay to detet reset
    _delay_ms(500);
    while(1)
    {
        //LED ON
    PORTD|=(1<<PD2);
    _delay_ms(20);
        //LED OFF
    PORTD&=~(1<<PD2);
        //~0.5s delay
    _delay_ms(500);
//	wdt_reset();
    }
	return (0);
}
