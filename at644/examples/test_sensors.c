#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "utils.h"
#include "motors.h"
#include "sensors.h"
#include "usart.h"
#include "logs.h"
#include "start.h"
#include "wd.h"

uint16_t	line_sens;
uint16_t	enemy_sens;
int16_t		a;
int16_t		b;
uint8_t		ca;
uint8_t		cb;
int			alert;

void main(void)
{
	uint8_t register i = 0;
	int	state = 1;
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(9600));
	#endif
	sei();
	sensors_init();
	third_board_init();
	uint8_t nbr;
	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		
		nbr = third_get_number();
		if (nbr < 7)
		{
			third_board_all(nbr, ENEMY(nbr));
		}
		else
		{
			third_board_all(nbr, LINE(nbr - 7));
		}
	
	#ifdef LOG
		log_enemy(enemy_sens);
		log_line(line_sens);
	#endif
	}
	return (0);
} 
