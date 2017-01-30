#define F_CPU 20000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xff;
	PORTB = 0;
	return (0);
}
