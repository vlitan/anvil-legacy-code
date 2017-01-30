#ifndef _DEFINES_H_
# define _DEFINES_H_

//states
# define ENEMY_AND_LINE_S	3
# define ON_ENEMY_S			2
# define ON_LINE_S			1
# define ON_NOTHING_S		0
//switches roles in normal mode
# define SPEED_SW0		2
# define SPEED_SW1		1
# define NO_ENEMY_SW	3
# define DEF_SPIN_SW	4
# define START_MODE_SW	5
# define CONTROL_SW		6
//return values in normal mode
//begin action
# define SEEK	0
# define SPIN	1
//must have the same value, begin arg to action
# define RIGHT		1
# define LEFT		2
# define FORWARD	1
# define BACKWARDS	2
//switches roles in control mode
# define START_Q_SW		1
# define SENS_Q_SW		2
# define LINE_Q_SW		4
# define ENEMY_Q_SW1	3
# define ENEMY_Q_SW0	4
//return values for control mode
# define DISPLAY_ENEMY1	1
# define DISPLAY_ENEMY2	2
# define DISPLAY_ENEMY3	3
# define DISPLAY_LINE1	4
# define DISPLAY_LINE2	5
# define DISPLAY_START	6
# define NOT_AN_OPTION	120
//swipe direction
# define LEFT_RIGHT		1
# define RIGHT_LEFT		2
//start mods
# define START_MODULE	1
# define WAIT_5_SEC		2

# define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)
#endif
