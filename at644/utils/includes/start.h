#ifndef _START_H_
# define _START_H_

# include <avr/interrupt.h>
# include <avr/io.h>
# include "utils.h"
# include <avr/wdt.h>
# include "global.h"

# define soft_reset()        \
 do                          \
 {                           \
     wdt_enable(WDTO_15MS);  \
     for(;;)                 \
     {                       \
     }                       \
 } while(0)

# define START_REG		D
# define START_PIN		4
# define START_PCIE		PCIE3
# define START_PCMSK	PCMSK3
# define START_PCINT	PCINT28

void	wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
void	wait_on_start(uint8_t);
void	start_init();

ISR (PCINT3_vect);

#endif

