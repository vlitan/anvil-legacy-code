#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"

#define ACK 'A'

/*********************************
all transmisions are ended with lf
all strings are ended with '\0'
if REMOVE_REPETITION is defined only new values are transmitted
***********************************/

/**************************************
*write a byte correnspondig with the current state.
*assuming there is a reference table
*format: "s<state_byte/>\n\0"
**************************************/
void	log_state(char st)
{
	static char last_st;
	#ifdef REMOVE_REPETITION
	if (last_st == st)
		return;
	last_st = st;
	#endif
	char s[4];
	
	s[0] = 's';
	if (st != 0)
		s[1] = st;
	else 
		s[1] = -1;
	s[2] = '\n';
	s[3] = 0;
	uart_puts(LOG_PORT, s);
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}

/**************************
*writes the comands sent to the motors (pwm values)
*params: int a, b represent the value sent to the function "go"
a,b C [-255, +255]
*each value (a / b) is sent on 2 bytes.
*first byte is for sign: 1 means +, 2 means -, 3 means 0
*second byte is the absolute value C [1, 255];
*format: "m<optA/><absA/><optB/><absB/>\n\0"
**************************/

void	write_motors(uint8_t write_port, int16_t a, int16_t b)
{
	char s[7];

	s[0] = 'm';
	if (a > 0)
	{
		s[1] = 1;
		s[2] = (a & 0xff);
	}
	else if (a < 0)
	{
		s[1] = 2;
		s[2] = ((-a) & 0xff);
	}
	else //a == 0
	{
		s[1] = 3;
		s[2] = 69; //random positive value
	}
	if (s[2] == '\n')
		s[2] = '\n' + 1;
	if (s[2] == '\r')
		s[2] = '\r' + 1;
	if (b > 0)
	{
		s[3] = 1;
		s[4] = (b & 0xff);
	}
	else if (b < 0)
	{
		s[3] = 2;
		s[4] = ((-b) & 0xff);
	}
	else //b == 0
	{
		s[3] = 3;
		s[4] = 69; //random positive value
	}
	if (s[4] == '\n')
		s[4] = '\n' + 1;
	if (s[4] == '\r')
		s[4] = '\r' + 1;
	s[5] = '\n';
	s[6] = 0;
	uart_puts(write_port, s);
}

 void	log_motors(int16_t a, int16_t b)
{
	static int16_t	last_a;
	static int16_t	last_b;
	#ifdef REMOVE_REPETITION
		if ((last_a == a) && (last_b == b))
			return;
		last_a = a;
		last_b = b;
	#endif

	write_motors(LOG_PORT, a, b);
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}

/*****************************
*sends the value of the enemy-sensors as bits (1 / 0).
*2 bytes are needed.
*the value 0 is forbidden.
*to ensure that even if the value to be sent
are 0 the MSB of each byte is set to 1 so there are 2 x 7 bits of data.
format: "e<1MSByte/><1LSByte>\n\0"
******************************/
void	log_enemy(uint16_t sens)
{
	static int last_enemy_sens;
	#ifdef REMOVE_REPETITION
	if (last_enemy_sens == sens)
		return;
	last_enemy_sens = sens;
	#endif

	char s[4];

	s[0] = 'e';
	// send the value with control bit
	s[1] = (sens & 0xff) | (1 << 7);
	s[2] = '\n';
	s[3] = 0;
	uart_puts(LOG_PORT, s);
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}

/***************************
*same thing as log_enemy, but MSB is not used, so its set to 1.
*format: "l<1LSByte/>\n\0"
****************************/
void	log_line(uint16_t sens)
{
	static int last_line_sens;
	#ifdef REMOVE_REPETITION
	//& 0x0f is a mask for the relevant bit
	if ((last_line_sens & 0x0f) == (sens & 0x0f))
		return;
	last_line_sens = sens;
	#endif

	char s[4];

	s[0] = 'l';
	s[1] = (sens & 0xff) | (1 << 7);
	s[2] = '\n';
	s[3] = 0;
	uart_puts(LOG_PORT, s);
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}

/**********************
prints a string and also adds lf
***********************/
char last_str[10];
void log_str(char *str)
{
	#ifdef REMOVE_REPETITION
		if (strcmp(last_str, str) == 0)
			return;
		strcpy(last_str, str);
	#endif
	uart_puts(LOG_PORT, str);
	uart_putc(LOG_PORT, '\n');
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}

void read_current(uint8_t *ca, uint8_t *cb)
{
	*ca = 0;
	*cb = 0;
	uart_putc(COMMAND_PORT, 'c');
	while (uart_getc(COMMAND_PORT) != 'c'); // wait for begining of resp.
	while (*ca == 0)
		*ca = uart_getc(COMMAND_PORT);
	while (*cb == 0)
		*cb = uart_getc(COMMAND_PORT);
	*ca -= 1;
	*cb -= 1;
}

void log_current(uint8_t ca, uint8_t cb)
{
	static uint8_t last_ca;	
	static uint8_t last_cb;
	#ifdef REMOVE_REPETITION
		if ((last_ca == ca) && (last_cb == cb))
			return;
		last_ca = ca;
		last_cb = cb;
	#endif
	char s[5];
	
	s[0] = 'c';
	s[1] = (ca == 255) ? 255 : (ca + 1);
	s[1] = (s[1] == '\n') ? (s[1] - 1) : s[1];
	s[2] = (cb == 255) ? 255 : (cb + 1);
	s[2] = (s[2] == '\n') ? (s[2] - 1) : s[2];
	s[3] = '\n';
	s[4] = 0;
	uart_puts(LOG_PORT, s);
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}

uint8_t read_alert(int8_t *alert)
{
	if (uart_getc(COMMAND_PORT) == 'a')
	{
		*alert = uart_getint(COMMAND_PORT);
		return (1);
	}
	return (0);
	
}

void log_alert(int8_t alert)
{
	uart_putc(LOG_PORT, 'a');
	uart_putint(LOG_PORT, alert);
	uart_putc(LOG_PORT, '\n');
	while (uart_getc(LOG_PORT) != ACK); //wait for ack byte
}
