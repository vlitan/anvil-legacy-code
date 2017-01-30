#ifndef _MOTORS_H_
# define _MOTORS_H_

# include "global.h"
# include "pwm.h"
# define  MOTORS_STEP_V  50
# define  MOTORS_STEP_MS 1000

/*
#ifdef SPEED_MANAGEMENT
	#if defined(SPEED_FACTOR)
		# define SPEED_FACTOR0 (_SPEED_FACTOR0 * SPEED_FACTOR)
		# define SPEED_FACTOR2 (_SPEED_FACTOR2 * SPEED_FACTOR)
	#else
		# define SPEED_FACTOR0 (_SPEED_FACTOR0)
		# define SPEED_FACTOR2 (_SPEED_FACTOR2)
	#endif
#endif
*/
#ifndef SEC_MCU
	void		go_0(int16_t speed);
	void		go_2(int16_t speed);
	inline void	motors_init(uint8_t mode, uint8_t prescale);
#endif

//void  ramp(int16_t *speed0, int16_t *speed2, unsigned long now_ms, uint8_t *state);
int16_t abs16 (int16_t val);
void ramp(int16_t *speed0, int16_t *speed2, unsigned long	ms);
void	set_motors_params(uint8_t limit, float factor);
void	go(int16_t speed0, int16_t speed2);
#endif
