/*moves inside the circle*/
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "defines.h"
#include "utils.h"
#include "motors.h"
#include "sensors.h"
#include "usart.h"
#include "logs.h"
#include "start.h"
#include "wd.h"
#include "misc.h"

//configure those values
#define	SPIN_FACTOR		150
#define	FORWARD_FACTOR	100
#define FORWARD_DEFAULT	150
#define	SPIN_CONST		170

extern uint8_t	speed_limit;	//stores the abs speed limit.
extern uint8_t	speed_factor;	//stores the speed factor

int16_t		speed_a;
int16_t		speed_b;


void main(void)
{
	/*init all the things!*/
	#ifdef LOG
		uart_init(LOG_PORT, BAUD_CALC(9600));
	#endif
	uart_init(COMMAND_PORT, BAUD_CALC(57600));
	sei();

	speed_limit = 250;
	speed_factor = 2;
	third_board_init();

/*	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);
	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);
	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);
	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);
	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);*/
	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);
	third_board_all(true);
	_delay_ms(1000);
	third_board_all(false);
	_delay_ms(1000);
	go(0, 0);			//make sure the robot is still

	while (1)
	{
		go(BLANA, 0);
		_delay_ms(1000);
		go(-BLANA, 0);
		_delay_ms(1000);
		go(0, BLANA);
		_delay_ms(1000);
		go(0, -BLANA);
		_delay_ms(1000);
		go(BLANA, BLANA);
		_delay_ms(10000);
		go(-BLANA, -BLANA);	
	}
}
