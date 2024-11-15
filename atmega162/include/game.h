#ifndef __GAME_MENU__
#define __GAME_MENU__

#include "joystick.h"
#include "menu.h"

/**
 * Initialize the menu and game states
 */
void GAME_init();

/**
 * Do one iteration of the game loop
 * Update OLED screen and game state
 * @param joystick direction
 * @param joystick button state
 * @param slider button state
 * @param game over flag
 * @return the current game state
 */
game_state_t GAME_loop(joystick_dir_t dir, int joystick_btn, int slider_btn,
                       int gameover);

#endif //__GAME_MENU__
