#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "sensors.h"
#include "usart.h"
#include "logs.h"


int	line_sens;
int enemy_sens;

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
	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		log_enemy(enemy_sens);
		log_line(line_sens);
		_delay_ms(1);	
	}
} 
