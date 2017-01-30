#define F_CPU 20000000UL
#include <global.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
<<<<<<< HEAD
#include <stdbool.h>
#include <utils.h>
#include <misc.h>
=======
//#include <utils.h>
//#include <misc.h>

//din alte fisiere
# define get_bit(nbr, bit_index)	((bool)((nbr) & (1 << (bit_index))))
inline void	set_reg_pin(volatile uint8_t *reg, uint8_t pin_index, bool pin_val);
uint8_t	third_board_get(uint8_t sw_index);
void third_board_set(uint8_t led_index, uint8_t val);
void third_board_init(void);

//////////////////
>>>>>>> a117dacef8097265f2afc655585ea4b49969bb21

void	third_light_one(uint8_t index);
void	blink_all(void);
void	swipe_1(void);
void	light_w_sw1(void);
void	light_w_sw2(void);

int main(void)
{
	third_board_init();
//pe rand..
<<<<<<< HEAD
//	blink_all();
//	swipe_1();
//	light_w_sw1();
	light_w_sw2();
=======
	//blink_all(); //tit of cat
	swipe_1();  //works fine
	//light_w_sw1();
	//light_w_sw2(); //n-am priceput ce vrea sa fie asta. nu compileaza

	/*
	//dirty test
    while (1)
	{
		third_board_set(0,third_board_get(5));
	}
	*/
>>>>>>> a117dacef8097265f2afc655585ea4b49969bb21

	return (0);
}

//**********de testat**********************
//test 1, clipesc toate ledurile
void	blink_all(void)
{
	uint8_t register state = 0;
	while (1)
	{
		third_board_set(1, state);
		third_board_set(2, state);
		third_board_set(3, state);
		_delay_ms(400);
		state = (state == 0) ? 1 : 0;
	}
}

//testul 2:ar trebui sa aprinda pe rand leduri in ordinea 1, 2, 3, 1[..]
void	swipe_1(void)
{
	uint8_t register i;
	while (1)
	{
		++i;
		if (i == 4)
		{
			i = 1;
		}
		third_light_one(i);
		_delay_ms(400);
	}
}
//daca nu se aprind toate inseamna ca ii mapat gresit si nu ar trebui sa mai continui (implicit un led ii mapat pe un sw si s-ar putea sa faca scurt sau ceva)

//daca unul din sw-uri ii on, toate is aprinse, altfel... nu.

///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//		GRESIT : butoanele indica 0 cand sunt ON
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

void	light_w_sw1(void)
{
	uint8_t register i;
	uint8_t register state;

	while (1)
	{
		state = 0;
		for (i = 1; i <= 6; ++i)
		{
			if (third_board_get(i) == 0)
			{
				state = 1; 
			}
		}
		third_board_set(1, state);
		third_board_set(2, state);
		third_board_set(3, state);
	}
}

//sw-urile conditioneaza ledurile dupa %

/*
void	light_w_sw2(void)
{
	uint8_t register i;
	while (1)
	{
		for (i = 1; i <= 6; ++i)
		{
			if (third_board_get(i))
			{
				third_light_one(i % 3 + 1);
			}
		}
	}
}
*/

//de ex, daca sw 1 sau 4 is on ledu 1 o sa fie on, 2 sau 5 pt 2, etc

//*******************************************

//aux
void	third_light_one(uint8_t index)
{
	third_board_set(1, 0);
	third_board_set(2, 0);
	third_board_set(3, 0);
	third_board_set(index, 1);
}
<<<<<<< HEAD
=======

//din alte fisiere
inline void	set_reg_pin(volatile uint8_t *reg, uint8_t pin_index, bool pin_val)
{
	if (pin_val)
		*reg |= 1<<pin_index;
	else
		*reg &= ~(1<<pin_index);
}

uint8_t	third_board_get(uint8_t sw_index)
{
	switch (sw_index)
	{
		case 1: return (get_bit(PINB, PB4));
		case 2: return (get_bit(PINB, PB3));
		case 3: return (get_bit(PIND, PD7));
		//case 4: return (get_bit(PINC, PC1));
		//case 5: return (get_bit(PINC, PC0));
		case 4: return (get_bit(PINC, PC0));
		case 5: return (get_bit(PINC, PC1));
		case 6: return (get_bit(PINB, PB0));
		default : return (0);
	}
}

void third_board_set(uint8_t led_index, uint8_t val)
{
	switch (led_index)
	{
		case 0: set_reg_pin(&PORTB, PB1, val); return; //left led
		case 1: set_reg_pin(&PORTB, PB2, val); return; //middle led
		case 2: set_reg_pin(&PORTD, PD6, val); return; //right led
	}
}

void third_board_init(void)
{
	/*inputs w/ pull ups*/
	set_reg_pin(&DDRB, PB0, 0);
	set_reg_pin(&PORTB, PB0, 1);
	set_reg_pin(&DDRC, PC1, 0);
	set_reg_pin(&PORTC, PC1, 1);
	set_reg_pin(&DDRD, PD6, 0);
	set_reg_pin(&PORTD, PD6, 1);
	set_reg_pin(&DDRD, PD7, 0);
	set_reg_pin(&PORTD, PD7, 1);
	set_reg_pin(&DDRC, PC0, 0);
	set_reg_pin(&PORTC, PC0, 1);
	set_reg_pin(&DDRB, PB3, 0);
	set_reg_pin(&PORTB, PB3, 1);
	set_reg_pin(&DDRB, PB4, 0);
	set_reg_pin(&PORTB, PB4, 1);
	/*outputs w/ default low*/
	//PD6 - right led
	set_reg_pin(&DDRD, PD6, 1);
	set_reg_pin(&PORTD, PD6, 0);
	//PB1 - left led
	set_reg_pin(&DDRB, PB1, 1);
	set_reg_pin(&PORTB, PB1, 0);
	//PB2 - middle led
	set_reg_pin(&DDRB, PB2, 1);
	set_reg_pin(&PORTB, PB2, 0);
}
>>>>>>> a117dacef8097265f2afc655585ea4b49969bb21
