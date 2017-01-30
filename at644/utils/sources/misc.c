#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"
#include "misc.h"
#include "defines.h"

void	third_board_all(uint8_t val)
{
	display_number((val) ? 7 : 0);
}

void	display_number(uint8_t nbr)
{
	third_board_set(1, nbr & 4);
	third_board_set(2, nbr & 2);
	third_board_set(3, nbr & 1);
}

void	display_state(uint8_t state)
{
	display_number(state);
}

uint8_t	third_get_number(void)
{
	volatile uint8_t res;
	uint8_t			 i;

	res = 0;
	for (i = 0; i < 6; ++i)
	{
		set_reg_pin(&res, i, third_board_get(i)); 
	}
}

uint8_t	third_board_get(uint8_t sw_index)
{
	switch (sw_index)
	{
		case 1: return (!get_bit(PINB, PB4));
		case 2: return (!get_bit(PINB, PB3));
		case 3: return (!get_bit(PIND, PD7));
		case 4: return (!get_bit(PINC, PC0));
		case 5: return (!get_bit(PINC, PC1));
		case 6: return (!get_bit(PINB, PB0));
		default : return (0);
	}
}

void third_board_set(uint8_t led_index, uint8_t val)
{
	switch (led_index)
	{
		case 1: set_reg_pin(&PORTB, PB1, val); break;
		case 2: set_reg_pin(&PORTB, PB2, val); break;
		case 3: set_reg_pin(&PORTD, PD6, val); break;
	}
}
void third_board_init(void)
{
	/*inputs w/ pull ups*/
	set_reg_pin(&DDRB, PB0, 0);
	set_reg_pin(&PORTB, PB0, 1);
	set_reg_pin(&DDRC, PC1, 0);
	set_reg_pin(&PORTC, PC1, 1);
	set_reg_pin(&DDRD, PD7, 0);
	set_reg_pin(&PORTD, PD7, 1);
	set_reg_pin(&DDRC, PC0, 0);
	set_reg_pin(&PORTC, PC0, 1);
	set_reg_pin(&DDRB, PB3, 0);
	set_reg_pin(&PORTB, PB3, 1);
	set_reg_pin(&DDRB, PB4, 0);
	set_reg_pin(&PORTB, PB4, 1);
	/*outputs w/ default low*/
	set_reg_pin(&DDRD, PD6, 1);
	set_reg_pin(&PORTD, PD6, 0);
	set_reg_pin(&DDRB, PB1, 1);
	set_reg_pin(&PORTB, PB1, 0);
	set_reg_pin(&DDRB, PB2, 1);
	set_reg_pin(&PORTB, PB2, 0);
}

void	third_board_only_one(uint8_t index)
{
	third_board_all(0);
	third_board_set(index, 1);	
}

uint8_t get_speed_factor(void)
{
	if (third_board_get(SPEED_SW1) && third_board_get(SPEED_SW0))
	{
		return (1);
	}
	else if (third_board_get(SPEED_SW1))
	{
		return (2);
	}
	else if (third_board_get(SPEED_SW0))
	{
		return (3);
	}
	else
	{
		return (4);
	}
}

uint8_t get_no_enemy(void)
{
	if (third_board_get(NO_ENEMY_SW))
	{
		return (SEEK);
	}
	else
	{
		return (SPIN);
	}
}

uint8_t	get_start_mode(void)
{
	if (third_board_get(START_MODE_SW))
	{
		return (START_MODULE);
	}
	else
	{
		return (WAIT_5_SEC);
	}
}

uint8_t get_default_dir(void)
{
	if (third_board_get(DEF_SPIN_SW))
	{
		return (LEFT);
	}
	else
	{
		return (RIGHT);
	}
}

uint8_t get_control(void)
{
	return (third_board_get(CONTROL_SW));
}

uint8_t	get_control_query(void)
{
	if (third_board_get(START_Q_SW))
	{
		return (DISPLAY_START);
	}
	if (third_board_get(SENS_Q_SW))
	{
		if (third_board_get(LINE_Q_SW))
		{
			return (DISPLAY_LINE1);
		}
		else
		{
			return (DISPLAY_LINE2);
		}
	}
	if ((third_board_get(ENEMY_Q_SW0)) && (third_board_get(ENEMY_Q_SW1)))
	{
		return (DISPLAY_ENEMY3);
	}
	else if (third_board_get(ENEMY_Q_SW0))
	{
		return (DISPLAY_ENEMY1);
	}
	else if (third_board_get(ENEMY_Q_SW1))
	{
		return (DISPLAY_ENEMY2);
	}
	return (NOT_AN_OPTION);
}

void	r328_init(void)
{
	set_reg_pin(&DDRD, PD5, 1);
	set_reg_pin(&PORTD, PD5, 1);
}

void	reset_328(void)
{
	set_reg_pin(&DDRD, PD5, 0);
	_delay_ms(1);
	set_reg_pin(&DDRD, PD5, 1);
}

void	swipe_leds(uint8_t way, uint8_t repeat)
{
	uint8_t	i;
	uint8_t	j;

	for (i = 0; i < repeat; ++i)
	{
		if (way == LEFT_RIGHT)
		{
			for (j = 1; j <= 3; ++j)
			{
				third_board_only_one(j);
				_delay_ms(90);
			}
		}
		else
		{
			for (j = 3; j >= 1; --j)
			{
				third_board_only_one(j);
				_delay_ms(90);
			}
		}
	}
}
