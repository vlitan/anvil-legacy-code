#ifndef _SENSORS_H_
# define _SENSORS_H_

# include "global.h"
# include <avr/io.h>

# define LINE_PORT		C	//IN port for reading line(+2enemy)
# define ENEMY_PORT		A	//IN port for reading enemy
# define LINE_COUNT		4
# define ENEMY_COUNT	7
# define LINE(x)		(!get_bit(line_sens, x))
# define ENEMY(x)		(!get_bit(enemy_sens, x))
# define ON_LINE		(0x0f ^ line_sens)
# define ON_ENEMY		(0x7f ^ enemy_sens)

# define BL 0
# define FL 1
# define FR 2
# define BR 3

inline void read_line_sens(uint16_t *line_sens);
inline void read_enemy_sens(uint16_t *enemy_sens);
inline void sensors_init(void);
#endif
