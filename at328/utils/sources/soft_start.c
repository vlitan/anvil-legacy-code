//DISCLAIMER:
//this is not the optimal implementation for a soft starter
#include "global.h"
#include "soft_start.h"
#include "misc.h"


int16_t last_speed_a;
int16_t last_speed_b;

void	manage_soft_start(int16_t *speed_a, int16_t *speed_b)
{
	*speed_a = mono_soft_start(*speed_a, last_speed_a);
	last_speed_a = *speed_a;
	*speed_b = mono_soft_start(*speed_b, last_speed_b);
	last_speed_b = *speed_b;
}

int16_t	mono_soft_start(int16_t next, int16_t last)
{
	//this is not perfect
	if (ABS(next - last) > SOFT_INCREMENT)
	{
		if ((SIGN(next) == SIGN(last)) && (ABS(next) < ABS(last)))
			return (next);
		else if (SIGN(next) != SIGN(last))
			return (last + SIGN(next) * SOFT_INCREMENT);
	}
	return (next);
}
