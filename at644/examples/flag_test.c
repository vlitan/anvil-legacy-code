#include "global.h"
#include "flag.h"
#include <util/delay.h>


int main(void)
{
	flag_init();
	set_flag(UP);
	_delay_ms(1000);
	while (1)
	{
		set_flag(RIGHT);
		_delay_ms(2000);
		set_flag(LEFT);
		_delay_ms(2000);
	}
	return (0);
}
