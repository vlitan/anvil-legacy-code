#include <avr/io.h>
#include "pwm.h"
#include "utils.h"

/*******************
this functions 
********************/

inline void	enable_pwm_out(uint8_t timer, uint8_t ab, _Bool state)
{
	switch (timer)
	{
		case 0:
				if (ab == A)
				{
					set_reg_pin(&TCCR0A, COM0A0, 0);
					set_reg_pin(&TCCR0A, COM0A1, state);
				}
				else if (ab == B)
				{
					set_reg_pin(&TCCR0A, COM0B0, 0);
					set_reg_pin(&TCCR0A, COM0B1, state);
				}
				break;
		case 1:
				if (ab == A)
				{
					set_reg_pin(&TCCR1A, COM1A0, 0);
					set_reg_pin(&TCCR1A, COM1A1, state);	
				}	
				else if (ab == B)
				{
					set_reg_pin(&TCCR1A, COM1B0, 0);
					set_reg_pin(&TCCR1A, COM1B1, state);
				}
				break;
		case 2:
				if (ab == A)
				{
					set_reg_pin(&TCCR2A, COM2A0, 0);
					set_reg_pin(&TCCR2A, COM2A1, state);
				}
				else if (ab == B)
				{
					set_reg_pin(&TCCR2A, COM2B0, 0);
					set_reg_pin(&TCCR2A, COM2B1, state);
				}
				break;
	}
}

/*********************
sets the match value for a specified timer and A/B out
**********************/

inline void	set_match_value(uint8_t timer, uint8_t ab, int match_value)
{
	if (timer == 0)
	{
		if (ab == A)
			OCR0A = match_value & 0xff;
		else if (ab == B)
			OCR0B = match_value & 0xff;
	}
	else if (timer == 1)
	{
		if (ab == A)
		{
			OCR1AH = match_value >> 8; 
			OCR1AL = match_value & 0xff;
		}
		else if (ab == B)
		{
			OCR1AH = match_value >> 8;
			OCR1AL = match_value & 0xff;
		}
	}
	else if (timer == 2)
	{
		if (ab == A)
			OCR2A = match_value & 0xff;
		else if (ab == B)
			OCR2B = match_value & 0xff;
	}
}

/******************************
very simple, not fancy at all.
all pwm are non-invertet
TOP is set by default at 0xFF (@8bit), 0x03FF(@10bit)
phase and freq corrected modes for 16bit timer are also omitted

main func. is set_timer
******************************/



inline void	set_timer_0(uint8_t mode, uint8_t prescaler)
{
	//set prescaler (32 and 128 missing at timer 1)
	switch (prescaler)
	{
		case PRESCALE_1: TCCR0B |= (1 << CS00); break;
		case PRESCALE_8: TCCR0B |= (1 << CS01); break;	
		case PRESCALE_64: TCCR0B |= (1 << CS01) | (1 << CS00);break;	
		case PRESCALE_256: TCCR0B |= (1 << CS02); break;	
		case PRESCALE_1024: TCCR0B |= (1 << CS02) | (1 << CS00); break;	
	}

	//set non-inverting mode
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	
	//set mode
	if (mode == FAST)
	{
		TCCR0A |= (1 << WGM00) | (1 << WGM01);
	}
	else if (mode == PHASE_CORRECT)
	{
		TCCR0A &= ~(1 << WGM01); //against residual values;
		TCCR0A |= (1 << WGM00);
	}
}

inline void	set_timer_1(uint8_t mode, uint8_t prescaler)
{
	//set prescaler
	switch (prescaler)
	{
		case PRESCALE_1: TCCR1B |= (1 << CS10); break;
		case PRESCALE_8: TCCR1B |= (1 << CS11); break;	
		case PRESCALE_64: TCCR1B |= (1 << CS11) | (1 << CS10);break;	
		case PRESCALE_256: TCCR1B |= (1 << CS12); break;	
		case PRESCALE_1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;	
	}
	
	//set non-inverting mode
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

	//set_mode
	if (mode == FAST)
	{
		TCCR1B |= (1 << WGM12);
		TCCR1A |= (1 << WGM11) || (1 << WGM10);
	}
	else if (mode == PHASE_CORRECT)
	{
		TCCR1B &= ~(1 << WGM12); //against residual values;
		TCCR1A |= (1 << WGM11) || (1 << WGM10);
	}
}

inline void	set_timer_2(uint8_t mode, uint8_t prescaler)
{
	//set prescaler
	switch (prescaler)
	{
		case PRESCALE_1: TCCR2B |= (1 << CS20); break;
		case PRESCALE_8: TCCR2B |= (1 << CS21); break;
		case PRESCALE_32: TCCR2B |= (1 << CS21) | (1 << CS20); break;
		case PRESCALE_64: TCCR2B |= (1 << CS22); break;
		case PRESCALE_128: TCCR2B |= (1 << CS22) | (1 << CS20); break;
		case PRESCALE_256: TCCR2B |= (1 << CS22) | (1 << CS21); break;	
		case PRESCALE_1024: TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); break;	
	}
	
	//set non-inverting mode
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
	
	//set_mode
	if (mode == FAST)
	{
		TCCR2A |= (1 << WGM20) | (1 << WGM21);
	}
	else if (mode == PHASE_CORRECT)
	{
		TCCR2A &= ~(1 << WGM21); //against residual values;
		TCCR2A |= (1 << WGM20);
	}
}

inline void	set_timer(uint8_t timer, uint8_t mode, uint8_t prescaler)
{
	if (timer == 0)
	{
		set_timer_0(mode, prescaler);
	}
	else if (timer == 1)
	{
		set_timer_1(mode, prescaler);
	}
	else if (timer == 2)
	{
		set_timer_2(mode, prescaler);
	}
}
