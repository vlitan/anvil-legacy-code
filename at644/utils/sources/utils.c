#include "utils.h"

inline void	set_reg_pin(volatile uint8_t *reg, uint8_t pin_index, bool pin_val)
{
	if (pin_val)
		*reg |= 1<<pin_index;
	else
		*reg &= ~(1<<pin_index);
}

inline void	toggle_reg_pin(volatile uint8_t *reg, uint8_t pin_index)
{
	*reg ^= (1 << pin_index);
}
