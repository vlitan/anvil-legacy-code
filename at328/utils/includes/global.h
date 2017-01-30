#ifndef _GLOBAL_H
# define _GLOBAL_H
#ifndef F_CPU
 #define F_CPU 16000000UL
#endif
# include <inttypes.h>

# define SPEED_MANAGEMENT
# define CURRENT_LIMIT 200

#ifdef SPEED_MANAGEMENT
	# define SPEED_LIMIT 200
//	# define SPEED_FACTOR 1.3
	# define _SPEED_FACTOR0 0.9
	# define _SPEED_FACTOR2 1
#endif

#endif
