#ifndef __MENU__
#define __MENU__

#include "adc.h"

typedef struct menu menu_t;
typedef struct menu_page menu_page_t;

typedef struct
{
  char *text;
  void (*cb)(menu_t *);
} menu_item_t;

typedef struct menu_page
{
  menu_page_t *parent;
  menu_item_t **items;
  int num_items;
  int selected;
  char *title;
} menu_page_t;

typedef enum
{
  MENU,
  GAME,
  GAME_OVER,
} game_state_t;

typedef struct menu
{
  menu_page_t *current;
  game_state_t state;
} menu_t;

/**
 * Update state of menu. E.G. change selected item
 * @param menu to be updated
 * @param dir direction of joystick
 */
void MENU_update_state(menu_t *menu, joystick_dir_t dir);

/**
 * Draw page to OLED
 * @param page to be drawn
 */
void MENU_draw_page(menu_page_t *page);

#endif // __MENU__
