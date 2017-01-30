#include <avr/io.h>
#include "global.h"
#include "motors.h"
#include "utils.h"
#include "pwm.h"
#include "soft_start.h"

/********************
go_x manages timer x
enables a channel and disables the complementary one
sets the match value for a channel and writes 0 on the other one
********************/

void	go_0(int16_t speed)
{
	if (speed > 0)
	{
		enable_pwm_out(0, A, 1);
		enable_pwm_out(0, B, 0);
		set_match_value(0, A, speed);
		set_reg_pin(&PORTD, 5, 0);
	}
	else
	{
		speed = -speed;
		enable_pwm_out(0, B, 1);
		enable_pwm_out(0, A, 0);
		set_match_value(0, B, speed);
		set_reg_pin(&PORTD, 6, 0);
	}
}

void	go_2(int16_t speed)
{
	if (speed > 0)
	{
		enable_pwm_out(2, A, 1);
		enable_pwm_out(2, B, 0);
		set_match_value(2, A, speed);
		set_reg_pin(&PORTD, 3, 0);
	}
	else
	{
		speed = -speed;
		enable_pwm_out(2, A, 0);
		enable_pwm_out(2, B, 1);
		set_match_value(2, B, speed);
		set_reg_pin(&PORTB, 3, 0);
	}
}

void	go(int16_t speed0, int16_t speed2)
{
	#ifdef SPEED_FACTOR0
		speed0 *= SPEED_FACTOR0;
	#endif
	#ifdef SPEED_FACTOR2
		speed2 *= SPEED_FACTOR2;
	#endif
	#ifdef SPEED_LIMIT
		speed0 = (speed0 > SPEED_LIMIT) ? SPEED_LIMIT : speed0;
		speed0 = (speed0 < -SPEED_LIMIT) ? -SPEED_LIMIT : speed0;
		speed2 = (speed2 > SPEED_LIMIT) ? SPEED_LIMIT : speed2;
		speed2 = (speed2 < -SPEED_LIMIT) ? -SPEED_LIMIT : speed2;
	#endif
//	manage_soft_start(&speed0, &speed2);
	go_0(speed0);
	go_2(speed2);
}


/*****************************
sets the used pins as outputs
sets timer 0 and 2 with mode and prescale
*****************************/
void	motors_init(uint8_t mode, uint8_t prescale)
{
	set_reg_pin(&DDRD, 3, 1);
	set_reg_pin(&DDRD, 5, 1);
	set_reg_pin(&DDRD, 6, 1);
	set_reg_pin(&DDRB, 3, 1);
	set_timer(0, mode, prescale);
	set_timer(2, mode, prescale);
	go(0, 0);
}
