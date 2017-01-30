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
	sensors_init();
	third_board_init();
	uint8_t nbr;
	while (1)
	{
		read_line_sens(&line_sens);
		read_enemy_sens(&enemy_sens);
		
		third_board_set(1, LINE(1)); //pui ce index vrei, maparea cu porturile o gasesti in sensors.c
	//	third_board_set(2, LINE(0)); // daca vrei sa testezi sens de linie, aceeasi treaba ca mai sus
		
		//incearca si asta, ar trebui sa-ti arate pe toate ledurile
		//third_board_all(ENEMY(2));	
	#ifdef LOG
		log_enemy(enemy_sens);
		log_line(line_sens);
	#endif
	}
	return (0);
} 

