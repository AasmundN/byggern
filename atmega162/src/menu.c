#include <stddef.h>

#include "menu.h"
#include "oled.h"

void MENU_update_state(menu_t *menu, joystick_dir_t dir)
{
  switch (dir)
  {
  case UP:
    menu->current->selected = menu->current->selected
                                  ? menu->current->selected - 1
                                  : menu->current->num_items - 1;
    break;

  case DOWN:
    menu->current->selected =
        menu->current->selected == menu->current->num_items - 1
            ? 0
            : menu->current->selected + 1;
    break;

  case LEFT:
    if (menu->current->parent != NULL)
      menu->current = menu->current->parent;
    break;

  case RIGHT:
    menu->current->items[menu->current->selected]->cb(menu);
    break;

  default:
    break;
  }
}

void MENU_draw_page(menu_page_t *page)
{
  OLED_clear_screen();

  OLED_print("________________", 0, 0);
  OLED_print(page->title, 0, 0);

  for (int i = 0; i < page->num_items && i < ROW_NUM - 2; i++)
    OLED_print(page->items[i]->text, i + 2, 0);

  OLED_write_char(MOGUS, page->selected + 2, COL_NUM - 1);

  OLED_refresh();
}
