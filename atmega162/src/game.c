#include "game.h"
#include "menu.h"
#include "oled.h"
#include "timer.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  char name[4];
  time_t score;
} score_t;
score_t leaderboard[4];

joystick_dir_t dir_prev = NEUTRAL;
time_t currentTime;
char timeStr[10];
uint8_t nameIndex = 0;
char name[4] = "AAA";

menu_item_t leaderboard_page_item_0;
menu_item_t leaderboard_page_item_1;
menu_item_t leaderboard_page_item_2;
menu_item_t leaderboard_page_item_3;
menu_item_t *leaderboard_page_items[4];

menu_page_t leaderboard_page;

menu_item_t credits_page_item_0;
menu_item_t credits_page_item_1;
menu_item_t credits_page_item_2;
menu_item_t *credits_page_items[3];

menu_page_t credits_page;

menu_item_t main_page_item_0;
menu_item_t main_page_item_1;
menu_item_t main_page_item_2;
menu_item_t *main_page_items[3];

menu_page_t main_page;

menu_t game_menu;

void default_cb(menu_t *menu)
{
  printf("%s \r\n", menu->current->items[menu->current->selected]->text);
}

void enter_credits_page_cb(menu_t *menu) { menu->current = &credits_page; }

void enter_leaderboard_page_cb(menu_t *menu)
{
  menu->current = &leaderboard_page;
  for (int i = 0; i < 4; i++)
  {
    char leaderboardStr[17];
    if (leaderboard[i].name[0] == '\0')
      break;
    sprintf(leaderboardStr, "#%d %s %2d:%2d", (i + 1), leaderboard[i].name,
            leaderboard[i].score.minutes, leaderboard[i].score.seconds);
    memcpy(leaderboard_page.items[i]->text, leaderboardStr, 17);
    printf("%s\r\n", leaderboardStr);
  }
}

void start_game_callback(menu_t *menu)
{
  menu->state = GAME;
  TIMER_reset_game_timer();
}

void GAME_init()
{

  // leaderboard page

  leaderboard_page_item_0.text = "#1                 ";
  leaderboard_page_item_0.cb = default_cb;

  leaderboard_page_item_1.text = "#2                 ";
  leaderboard_page_item_1.cb = default_cb;

  leaderboard_page_item_2.text = "#3                 ";
  leaderboard_page_item_2.cb = default_cb;

  leaderboard_page_item_3.text = "#4                 ";
  leaderboard_page_item_3.cb = default_cb;

  leaderboard_page_items[0] = &leaderboard_page_item_0;
  leaderboard_page_items[1] = &leaderboard_page_item_1;
  leaderboard_page_items[2] = &leaderboard_page_item_2;
  leaderboard_page_items[3] = &leaderboard_page_item_3;

  leaderboard_page.parent = &main_page;
  leaderboard_page.items = leaderboard_page_items;
  leaderboard_page.num_items = 4;
  leaderboard_page.selected = 0;
  leaderboard_page.title = "Leaderboard";

  // credits page

  credits_page_item_0.text = "Aasmund";
  credits_page_item_0.cb = default_cb;

  credits_page_item_1.text = "Tobias";
  credits_page_item_1.cb = default_cb;

  credits_page_item_2.text = "William";
  credits_page_item_2.cb = default_cb;

  credits_page_items[0] = &credits_page_item_0;
  credits_page_items[1] = &credits_page_item_1;
  credits_page_items[2] = &credits_page_item_2;

  credits_page.parent = &main_page;
  credits_page.items = credits_page_items;
  credits_page.num_items = 3;
  credits_page.selected = 0;
  credits_page.title = "Credits";

  // main page

  main_page_item_0.text = "Play";
  main_page_item_0.cb = start_game_callback;

  main_page_item_1.text = "Leaderboard";
  main_page_item_1.cb = enter_leaderboard_page_cb;

  main_page_item_2.text = "Credits";
  main_page_item_2.cb = enter_credits_page_cb;

  main_page_items[0] = &main_page_item_0;
  main_page_items[1] = &main_page_item_1;
  main_page_items[2] = &main_page_item_2;

  main_page.parent = NULL;
  main_page.items = main_page_items;
  main_page.num_items = 3;
  main_page.selected = 0;
  main_page.title = "Main menu";

  // menu

  game_menu.current = &main_page;
  game_menu.state = MENU;
}

game_state_t GAME_loop(joystick_dir_t dir, int joystick_btn, int slider_btn,
                       int gameover)
{
  if (slider_btn)
    game_menu.state = GAME_OVER;

  if (gameover && game_menu.state == GAME)
    game_menu.state = GAME_OVER;

  switch (game_menu.state)
  {
  case GAME:
    OLED_clear_screen();
    OLED_print("Game running", 0, 0);

    time_t currentTimeLocal = TIMER_get_game_time();
    memcpy(&currentTime, &currentTimeLocal, sizeof(time_t));

    sprintf(timeStr, "%2u:%2u", currentTime.minutes, currentTime.seconds);
    OLED_print("Time: ", 3, 0);
    OLED_print(timeStr, 3, 6);

    OLED_refresh();
    break;

  case MENU:
    if (dir != dir_prev)
      MENU_update_state(&game_menu, dir);

    MENU_draw_page(game_menu.current);
    dir_prev = dir;

    break;

  case GAME_OVER:
    OLED_clear_screen();

    OLED_print("Game over", 0, 0);
    OLED_print("Time: ", 3, 0);
    OLED_print(timeStr, 3, 6);

    OLED_print("Enter name:", 5, 0);
    OLED_print(name, 6, 0);
    OLED_write_char(MOGUS, 7, nameIndex);

    if (!joystick_btn)
    {
      game_menu.state = MENU;
      int leaderboardIndex = 4;

      for (int i = 0; i < 4; i++)
      {
        if (leaderboard[i].score.minutes < currentTime.minutes)
        {
          leaderboardIndex = i;
          break;
        }

        if (leaderboard[i].score.minutes == currentTime.minutes
            && leaderboard[i].score.seconds < currentTime.seconds)
        {
          leaderboardIndex = i;
          break;
        }
      }

      if (leaderboardIndex < 4)
      {
        for (int i = 3; i > leaderboardIndex; i--)
        {
          memcpy(&leaderboard[i], &leaderboard[i - 1], sizeof(score_t));
        }

        memcpy(&leaderboard[leaderboardIndex].name, &name, sizeof(char) * 4);
        memcpy(&leaderboard[leaderboardIndex].score, &currentTime,
               sizeof(time_t));
      }

      game_menu.state = MENU;
      for (int i = 0; i < 4; i++)
      {
        printf("#%u %s %2u:%2u \r\n", (i + 1), leaderboard[i].name,
               leaderboard[i].score.minutes, leaderboard[i].score.seconds);
      }
    }

    if (dir != dir_prev)
      switch (dir)
      {
      case LEFT:
        nameIndex = (nameIndex - 1) % 3;
        break;

      case RIGHT:
        nameIndex = (nameIndex + 1) % 3;
        break;

      case UP:
        name[nameIndex]--;
        break;

      case DOWN:
        name[nameIndex]++;
        break;

      default:
        break;
      }

    dir_prev = dir;
    OLED_refresh();

    break;

  default:
    break;
  }

  return game_menu.state;
}
