#ifndef __GAME_MENU__
#define __GAME_MENU__

#include "menu.h"
#include "adc.h"

void GAME_init();

game_state_t GAME_loop(joystick_dir_t dir, int joystick_btn, int slider_btn, int gameover); 

#endif //__GAME_MENU__
