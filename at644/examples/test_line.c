#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "utils.h"
#include "sensors.h"
#include "misc.h"


uint16_t line_sens;

void main(void)
{
	uint8_t register i;
	uint8_t state;

	/*init all the things!*/
	third_board_init();
	sensors_init();
	while (1)
	{
		read_line_sens(&line_sens);
		third_board_set(3, LINE(0));
	/*	if (ON_LINE)
		{
			if (LINE(FL) || LINE(FR))
			{
				speed_a = 255;
				speed_b = 255;
			}
			else if (LINE(BL) || LINE(BR))
			{
				speed_a = -255;
				speed_b = -255;
			}
		}
		go(speed_a, speed_b);*/
	}
	return (0);
} 
