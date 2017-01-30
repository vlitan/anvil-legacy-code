
#include "start.h"
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"

//disable watchdog (used by soft_reset)
void	wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();
    return;
}


void	wait_on_start(uint8_t mode)
{
	if (mode == START_MODULE)
	{
		while (get_bit(PIN(START_REG), START_PIN) == 0);
	}
	else
	{
		_delay_ms(3000);
	}
}

void	start_init()
{
	//set as input
	set_reg_pin(&DDR(START_REG), START_PIN, 0);
	//enable internall pull-up
//	set_reg_pin(&PORT(START_REG), START_PIN, 0);
	//enable interrupt on mask
	set_reg_pin(&PCICR, START_PCIE, 1);
	//enable interrupt on pin
	set_reg_pin(&START_PCMSK, START_PCINT, 1);
}

ISR (PCINT3_vect)
{
//	PORTB = 0xff;
//	_delay_ms(1000);
//	PORTB = 0;
	if (get_bit(PIN(START_REG), START_PIN) == 0)
	{
		soft_reset();
	}
}

