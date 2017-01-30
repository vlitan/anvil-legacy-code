#ifndef _MOTORS_H_
# define _MOTORS_H_
# include "pwm.h"
# include "global.h"

#ifdef SPEED_MANAGEMENT
	#if defined(SPEED_FACTOR)
		# define SPEED_FACTOR0 (_SPEED_FACTOR0 * SPEED_FACTOR)
		# define SPEED_FACTOR2 (_SPEED_FACTOR2 * SPEED_FACTOR)
	#else
		# define SPEED_FACTOR0 (_SPEED_FACTOR0)
		# define SPEED_FACTOR2 (_SPEED_FACTOR2)
	#endif
#endif

void		go_0(int16_t speed);
void		go_2(int16_t speed);
inline void	go(int16_t speed0, int16_t speed1);
inline void	motors_init(uint8_t mode, uint8_t prescale);
#endif
