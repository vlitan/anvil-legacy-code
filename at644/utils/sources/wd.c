#include "global.h"
#include "start.h"
#include "wd.h"
#include "logs.h"
#include "misc.h"

///4 debug only
#include <util/delay.h>
#include <avr/io.h>

void WDT_Init(void)
{
//disable interrupts
cli();
//reset watchdog
wdt_reset();
//set up WDT interrupt
WDTCSR = (1<<WDCE)|(1<<WDE);
//Start watchdog timer with ~0.5s timeout
WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP2)|(1<<WDP0);
//Enable global interrupts
sei();
}

//Watchdog timeout ISR
ISR(WDT_vect)
{
	//action before reset
	#ifdef LOG
		log_str("RESET!");
	#endif
	reset_328();
	//occurs after 15ms
	soft_reset();
}
