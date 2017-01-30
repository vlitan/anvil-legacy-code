#define F_CPU 20000000UL

#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <utils.h>
#define LED1	3
#define LED2	4
#define BUTT1	5
#define BUTT2	6
#define soft_reset()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)

// Function Pototype
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));


int main(void)
{

	//outputs
	set_reg_pin(&DDRB, LED1, 1);
	set_reg_pin(&DDRB, LED2, 1);
	set_reg_pin(&PORTB, LED2, 1);
	set_reg_pin(&PORTB, LED1, 1);
	//inputs
	set_reg_pin(&DDRB, BUTT2, 0);
	set_reg_pin(&PORTB, BUTT2, 1); //internall pull-up
	set_reg_pin(&DDRB, BUTT1, 0);
	set_reg_pin(&PORTB, BUTT1, 1); //internall pull-up
	
	//piruete incipient-demonstrative
	_delay_ms(200);
	toggle_reg_pin(&PORTB, LED1);
	_delay_ms(200);
	toggle_reg_pin(&PORTB, LED1);
	_delay_ms(200);
	toggle_reg_pin(&PORTB, LED2);
	
	set_reg_pin(&PCICR, PCIE1, 1); //enable interrupts on PCINT1
	set_reg_pin(&PCMSK1, PCINT13, 1);
	
	sei();
	
	while (get_bit(PINB, BUTT1) == 0);

	while (1)
	{
		toggle_reg_pin(&PORTB, LED1);
		_delay_ms(100);
	}	
	return (0);
}

ISR (PCINT1_vect)
{
	if (get_bit(PINB, BUTT1) == 0)
	{
		soft_reset();
	}
}


