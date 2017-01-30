#include "global.h"
#include <avr/io.h>
#include "utils.h"
#include "pwm.h"
#include "logs.h"
#include "usart.h"
#include "motors.h"
#include <math.h>
#define SEC_MCU

#define ACK 'A'

uint8_t	speed_limit;
uint8_t	speed_factor;

/********************
go_x manages timer x
enables a channel and disables the complementary one
sets the match value for a channel and writes 0 on the other one
********************/

#ifndef SEC_MCU
void		go_0(int16_t speed)
{
	if (speed > 0)
	{
		enable_pwm_out(0, A, 1);
		enable_pwm_out(0, B, 0);
		set_match_value(0, A, speed);
		set_reg_pin(&PORTB, 4, 0);
	}
	else
	{
		speed = -speed;
		enable_pwm_out(0, B, 1);
		enable_pwm_out(0, A, 0);
		set_match_value(0, B, speed);
		set_reg_pin(&PORTB, 3, 0);
	}
}

void		go_2(int16_t speed)
{
	if (speed > 0)
	{
		enable_pwm_out(2, A, 1);
		enable_pwm_out(2, B, 0);
		set_match_value(2, A, speed);
		set_reg_pin(&PORTD, 6, 0);
	}
	else
	{
		speed = -speed;
		enable_pwm_out(2, A, 0);
		enable_pwm_out(2, B, 1);
		set_match_value(2, B, speed);
		set_reg_pin(&PORTD, 7, 0);
	}
}

/*****************************
sets the used pins as outputs
sets timer 0 and 2 with mode and prescale
*****************************/
inline void	motors_init(uint8_t mode, uint8_t prescale)
{
	set_reg_pin(&DDRB, 3, 1);
	set_reg_pin(&DDRB, 4, 1);
	set_reg_pin(&DDRD, 6, 1);
	set_reg_pin(&DDRD, 7, 1);
	set_timer(0, mode, prescale);
	set_timer(2, mode, prescale);
}
#endif

int16_t abs16 (int16_t val)
{
	if (val < 0)
		return -val;
	return val;
}
unsigned long last_ms;
int16_t last_s0, last_s2;
uint8_t first;
void ramp(int16_t *speed0, int16_t *speed2, unsigned long	ms)
{
	if (first == 0)
	{
		last_s0 = *speed0;
		last_s2 = *speed2;
		first = 1;
	}
	if ((ms - last_ms) >= MOTORS_STEP_MS)
	{
		if (abs16(*speed0 - last_s0) >= MOTORS_STEP_V)
		{
			if (*speed0 - last_s0 > 0)
			{
				*speed0 = last_s0 + MOTORS_STEP_V;
			}
			else
			{
				*speed0 = last_s0 - MOTORS_STEP_V;
			}
		}
		if (abs16(*speed2 - last_s2) >= MOTORS_STEP_V)
		{
			if (*speed2 - last_s2 > 0)
			{
				*speed2 = last_s2 + MOTORS_STEP_V;
			}
			else
			{
				*speed2 = last_s2 - MOTORS_STEP_V;
			}
		}
		last_ms = ms;
		last_s0 = *speed0;
		last_s2 = *speed2;
		}
}


void	go(int16_t speed0, int16_t speed2)
{
//	speed0 = (int16_t)ceil((MOTOR_A_FACTOR * speed0 + MOTOR_A_ADDER) / speed_factor);
//	speed2 = (int16_t)ceil((MOTOR_B_FACTOR * speed2 + MOTOR_B_ADDER) / speed_factor);

	speed0 = (int16_t)ceil(speed0 / speed_factor);
	speed2 = (int16_t)ceil(speed2 / speed_factor);
	speed0 = (speed0 > speed_limit) ? speed_limit : speed0;
	speed0 = (speed0 < -speed_limit) ? -speed_limit : speed0;
	speed2 = (speed2 > speed_limit) ? speed_limit : speed2;
	speed2 = (speed2 < -speed_limit) ? -speed_limit : speed2;
	#ifdef LOG
		log_motors(speed0, speed2);
	#endif

	#ifdef SEC_MCU
		//write_motors(COMMAND_PORT, speed0, speed2);
		//while (uart_getc(COMMAND_PORT) != ACK);
		uart_putc(COMMAND_PORT, 35);
		uart_putc(COMMAND_PORT, 35);
		uart_putc(COMMAND_PORT, 35);
		uart_putc(COMMAND_PORT, speed0 & 0xff);
		uart_putc(COMMAND_PORT, (speed0 >> 8) & 0xff);
		uart_putc(COMMAND_PORT, (speed2 & 0xff));
		uart_putc(COMMAND_PORT, (speed2 >> 8) & 0xff);
		uart_putc(COMMAND_PORT, 0);
		uart_putc(COMMAND_PORT, 0);
	#endif
/*	#else
		go_0(speed0);
		go_2(speed2);
	#endif*/
}


/*
void ramp_one(int16_t *speed, int16_t *last)
{
	if (abs(*speed - *last) > MOTORS_STEP_V)
	{
		if (*speed < *last)
		{
			*speed = *last - MOTORS_STEP_V;
		}
		else if (*speed > *last)
		{
			*speed = *last + MOTORS_STEP_V;
		}
	}
	*last = *speed;
}

void ramp(int16_t *speed0, int16_t *speed2, unsigned long now_ms, uint8_t *state) {
	static int16_t last0;
	static int16_t last2;
	static unsigned long last_now;

	if (now_ms - last_now > MOTORS_STEP_MS)
	{
		ramp_one(speed0, &last0);
		ramp_one(speed2, &last2);
		if (*state == 0)
		{
			*state = 7;
		}
		else
		{
			*state = 0;
		}
			last_now = now_ms;
	}
}*/
