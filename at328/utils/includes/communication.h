#ifndef _COMUNNICATION_H_
# define _COMMUNICATION_H_

# define CURRENT_REQ 1
# define MOTORS_SET  2
# define NONE		 0
# define ACK		'A'

void		write_current(uint8_t ca, uint8_t cb);
void		write_alert(uint8_t sd);
inline void	read_motors(int16_t *a, int16_t *b);
#endif
