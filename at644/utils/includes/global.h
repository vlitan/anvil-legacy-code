#ifndef _GLOBAL_H
# define _GLOBAL_H

#ifndef F_CPU
	# define F_CPU 20000000UL
#endif

//# define LOG				// if defined data is logged to the PC
# define SEC_MCU			//if defined motors comands are transmitted to another MCU
# define REMOVE_REPETITION	//manages logging data, only new data is sent
/* this is needed in case of mechanical issues, to calbrate the motors
# define SPEED_MANAGEMENT
#ifdef SPEED_MANAGEMENT
	# define SPEED_LIMIT 250	//limits the speed
	# define SPEED_FACTOR (1)	//proportional speed adjusting on both motors
	# define _SPEED_FACTOR0 0.9	//only on timer 0
	# define _SPEED_FACTOR2 1	//only on timer 2
#endif
*/

# define BLANA	255

# define COMMAND_PORT	0	//UART port for sending comands
# define LOG_PORT		1	//UART port for logging data

# define MOTOR_A_FACTOR 1
# define MOTOR_B_FACTOR 1
# define MOTOR_A_ADDER	0
# define MOTOR_B_ADDER	0
#endif
