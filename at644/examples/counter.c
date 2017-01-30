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

extern uint8_t	speed_limit;
extern float	speed_factor;

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
	uart_init(LOG_PORT, BAUD_CALC(9600));
	sei();
	sensors_init();
	third_board_init();
	uart_puts(LOG_PORT, "qwase\n");

	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
/*		if (LINE(0) && state)
		{
			++i;
			state = 0;
		}
		else if (!LINE(0))
		{
			state = 1;
		}
		if (ENEMY(2))
		{
			i = 0;
		}	
		
		third_board_set(3, i & 1);
		third_board_set(2, i & 2);
		third_board_set(1, i & 4);
*/
		log_enemy(enemy_sens);
		log_line(line_sens);
	}
	return (0);
} 
