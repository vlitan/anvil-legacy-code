#include "global.h"
#include <avr/io.h>
#include "pwm.h"
#include "utils.h"
#include "flag.h"
#include <stdint.h>

void	flag_init(void)
{
	set_reg_pin(&DDRD, PD6, 1);
	set_timer(2, PHASE_CORRECT, PRESCALE_1024);
}

void	set_flag(uint8_t pos)
{
	set_match_value(2, B, pos);
}
