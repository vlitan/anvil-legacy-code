#ifndef _WD_H_
# define _WD_H_
# include <avr/interrupt.h>
# include <avr/wdt.h>
void	wdt_init(uint8_t interval);
#endif
