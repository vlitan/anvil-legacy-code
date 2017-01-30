#include "global.h"
#include "utils.h"
#include "sensors.h"
#include <util/delay.h>
#include <avr/io.h>
/*
>puts the 4 values from the line sensors
on the first 4 bits of ls
>0x3C is a mask to get the values from the PIN register
>the result has to be shifted to the right
*/
//C2 - index: 0
//C3 - index: 1
//C4 - index: 2
//C5 - index: 3

void read_line_sens(uint16_t *ls)
{
	uint16_t last2, last = 0xffff;
	*ls = (PIN(LINE_PORT) & 0x3C) >> 2;
	while ((last2 != *ls) && (last != *ls))
	{
		last2 = last;
		last = *ls;
		*ls = (PIN(LINE_PORT) & 0x3C) >> 2;
		_delay_us(10);
	}
}


/*
puts the 7 values from the enemy sensors
on the first 7 bites of es ("borrow" 1 bit from the line port)
*/
//A2 - index: 0
//A3 - index: 1
//A4 - index: 2
//A5 - index: 3
//A6 - index: 4
//A7 - index: 5
//C7 - index: 6
//C6 - index: 7
inline void read_enemy_sens(uint16_t *es)
{
	*es = (PIN(ENEMY_PORT) >> 2) | ((PIN(LINE_PORT) & 0x80) >> 1);
}

inline void line_sens_isr_init()
{
	set_reg_pin(&PCICR, PCIE2, 1); 
	set_reg_pin(&PCMSK2, PCINT18, 1); 
	set_reg_pin(&PCMSK2, PCINT19, 1); 
	set_reg_pin(&PCMSK2, PCINT20, 1); 
	set_reg_pin(&PCMSK2, PCINT21, 1); 
}


inline void	sensors_init(void)
{
	uint8_t register i;
	for (i = 2; i < 8; ++i)
	{	
		//set as input
		set_reg_pin(&DDR(ENEMY_PORT), i, 0);
		set_reg_pin(&DDR(LINE_PORT), i, 0);
		//set internal pull-up
		set_reg_pin(&PORT(ENEMY_PORT), i, 1);
		set_reg_pin(&PORT(LINE_PORT), i, 1);
 	}
	line_sens_isr_init();
}
