#ifndef __GAME_MENU__
#define __GAME_MENU__

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "timer.h"

typedef struct {
    char name[4];
    time_t score;
  } score_t;
score_t leaderboard[4];

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

void enter_leaderboard_page_cb(menu_t *menu) {
  menu->current = &leaderboard_page;
  for (int i = 0; i < 4; i++) {
    char leaderboardStr[17];
    sprintf(leaderboardStr,"#%d %s %2d:%2d", (i+1), leaderboard[i].name, leaderboard[i].score.minutes, leaderboard[i].score.seconds);
    memcpy(leaderboard_page.items[i]->text, leaderboardStr, strlen(leaderboard_page.items[i]->text));
    printf("%s\r\n", leaderboardStr);
  }
}

void start_game_callback(menu_t *menu){
  menu->state = GAME;
  TIMER_reset_game_timer();
}

void GAME_MENU_init()
{


  // leaderboard page

  leaderboard_page_item_0.text = "                 ";
  leaderboard_page_item_0.cb = default_cb;

  leaderboard_page_item_1.text = "                 ";
  leaderboard_page_item_1.cb = default_cb;

  leaderboard_page_item_2.text = "                 ";
  leaderboard_page_item_2.cb = default_cb;

  leaderboard_page_item_3.text = "                 ";
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

#endif //__GAME_MENU__
