#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "usart.h"
#include "logs.h"
#include "motors.h"

void main(void)
{
	uart_init(LOG_PORT, BAUD_CALC(115200));
	uart_init(COMMAND_PORT, BAUD_CALC(115200));
	sei();
	int i = -255;
	char c;
	//write_motors(COMMAND_PORT, 200, 200);
//	go(240, 240);
//	go(5, 10);
//	_delay_ms(1000);
//	go(-200, -200);

	while (1)
	{
	go (127, 127);
//		i++;
//		go(i, i);
//		if (i >= 255)
//			i = -255;
//		_delay_ms(20);
	}
} 
