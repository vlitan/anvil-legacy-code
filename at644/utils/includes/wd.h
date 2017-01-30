#ifndef _WD_H_
# define _WD_H_
#include <avr/interrupt.h>
#include <avr/wdt.h>
void	WDT_Init(void);
ISR (WDT_vect);
#endif
