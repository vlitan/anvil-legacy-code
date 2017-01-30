#ifndef _LOGS_H_
# define _LOGS_H_

# include "global.h"
# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>
# include <stdio.h>
# include "usart.h"

void	log_state(char);
void	log_enemy(uint16_t);
void	log_line(uint16_t);
void	log_str(char*);
void	write_motors(uint8_t, int16_t, int16_t);
void	log_motors(int16_t, int16_t);
void	read_current(uint8_t*, uint8_t*);
uint8_t	read_alert(uint8_t*);
void	log_current(uint8_t, uint8_t);
#endif
