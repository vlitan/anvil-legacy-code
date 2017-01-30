#ifndef _PWM_H_
# define _PWM_H_
# define FAST 1
# define PHASE_CORRECT 2
# define PHASE_FREQ_CORRECT 3
# define PRESCALE_1 1
# define PRESCALE_8 2
# define PRESCALE_32 3
# define PRESCALE_64 4
# define PRESCALE_128 5
# define PRESCALE_256 6
# define PRESCALE_1024 7
# define A 1
# define B 2
# include "global.h"

inline void	set_timer(uint8_t timer, uint8_t mode, uint8_t prescaler);
inline void	set_match_value(uint8_t timer, uint8_t ab, int match_value);
inline void	enable_pwm_out(uint8_t timer, uint8_t ab, _Bool state);
#endif
