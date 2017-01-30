#ifndef _MISC_H_
# define _MISC_H_
void	r328_init(void);
void	display_state(uint8_t);
void	display_number(uint8_t);
void	reset_328(void);
void	third_board_set(uint8_t, uint8_t);
void	third_board_init(void);
void	third_board_only_one(uint8_t);
void	swipe_leds(uint8_t, uint8_t);
uint8_t third_board_get(uint8_t);
uint8_t get_speed_factor(void);
uint8_t get_no_enemy(void);
uint8_t get_default_dir(void);
uint8_t	get_control(void);
uint8_t	get_control_query(void);
uint8_t	get_start_mode(void);
#endif
