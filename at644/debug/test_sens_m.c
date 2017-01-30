#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "utils.h"
#include "sensors.h"
#include "usart.h"
#include "logs.h"


int	line_sens;
int enemy_sens;

extern uint8_t	speed_limit;
extern float	speed_factor;

void main(void)
{
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(115200));
	#endif
	#ifdef SEC_MCU
		uart_init(COMMAND_PORT, BAUD_CALC(115200));
	#endif
	#if defined(LOG) || defined(SEC_MCU)
		sei();
	#endif
	set_sensors();
	speed_limit = 130;
	speed_factor = 0.5;
	register uint8_t i;
	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		log_enemy(enemy_sens);
		log_line(line_sens);
		for (i = 0; i < 7; ++i)
			if (!get_bit(line_sens, i))
				go(0, 0);
		for (i = 0; i < 9; ++i)
			if (!get_bit(enemy_sens, i))
				go(400, 150);
		_delay_ms(1);	
	}
} 
