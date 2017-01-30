#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "utils.h"
#include "motors.h"
#include "usart.h"
#include "communication.h"
#include "analog.h"
#include "wd.h"

int16_t speed_a, last_speed_a;
int16_t speed_b, last_speed_b;
int16_t current_a;
int16_t current_b;
uint8_t	req_t;
uint8_t over_current_a;
uint8_t over_current_b;

int main(void)
{
	/*init all the things*/
	motors_init(PHASE_CORRECT, PRESCALE_8);
	uart_init(BAUD_CALC(9600));
	sei();
	adc_init();
	wdt_init(WDTO_500MS);
	/********************/
	go(0, 0);
	uart_putc(ACK);
	over_current_a = 0;
	over_current_b = 0;
	while (1)
	{
		current_a = analog_read(C_SENS_A);
		current_b = analog_read(C_SENS_B);

		req_t = get_request_type();
		if (req_t == CURRENT_REQ)
		{	
			write_current(current_a, current_b);
		}
		else if (req_t == MOTORS_SET)
		{
			read_motors(&speed_a, &speed_b);
			wdt_reset();
		}
		
		//manage current sens
		#ifdef CURRENT_LIMIT
		if ((current_a < CURRENT_LIMIT) && (current_b < CURRENT_LIMIT))
		{
			go(speed_a, speed_b);
			over_current_a = over_current_b = 0;
		}
		else
		{
			//overcurrent
			go(0, 0);
			uart_putc('!');
			over_current_a = current_a - CURRENT_LIMIT;
			over_current_b = current_b - CURRENT_LIMIT;
		}
		#endif
	}
	return (0);
}
