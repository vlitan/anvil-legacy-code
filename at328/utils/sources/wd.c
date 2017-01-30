#include "wd.h"

void wdt_init(uint8_t interval)
{
	cli();
	WDTCSR = 0;
	wdt_reset();
	//the wd works in reset mode
	WDTCSR = (1 << WDCE) | (1 << WDE);
	wdt_enable(interval);
	sei();
}

