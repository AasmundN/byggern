#ifndef __GAME_MENU__
#define __GAME_MENU__

#include <stddef.h>
#include <stdio.h>

#include "menu.h"

menu_item_t child_page_item_0;
menu_item_t child_page_item_1;
menu_item_t* child_page_items[2];

menu_page_t child_page;

menu_item_t parent_page_item_0;
menu_item_t parent_page_item_1;
menu_item_t parent_page_item_2;
menu_item_t* parent_page_items[3];

menu_page_t parent_page;

menu_t game_menu;

void default_cb(menu_t* menu){
    printf(menu->current->items[menu->current->selected]->text);
}

void enter_child_page_cb(menu_t* menu){
    menu->current = &child_page;
}


void GAME_MENU_init(){
    child_page_item_0.text = "Sugus",
    child_page_item_0.cb = default_cb;

    child_page_item_1.text = "Gomus";
    child_page_item_1.cb = default_cb;

    child_page_items[0] = &child_page_item_0;
    child_page_items[1] = &child_page_item_1;

    child_page.parent = &parent_page;
    child_page.items = child_page_items;
    child_page.num_items = 2;
    child_page.selected = 0;
    child_page.title = "Child";


    parent_page_item_0.text = "Su";
    parent_page_item_0.cb = default_cb;

    parent_page_item_1.text = "Go";
    parent_page_item_1.cb = enter_child_page_cb;

    parent_page_item_2.text = "Ma";
    parent_page_item_2.cb = default_cb;

    parent_page_items[0] = &parent_page_item_0;
    parent_page_items[1] = &parent_page_item_1;
    parent_page_items[2] = &parent_page_item_2;

    parent_page.parent = NULL;
    parent_page.items = parent_page_items;
    parent_page.num_items = 3;
    parent_page.selected = 0;
    parent_page.title = "Parent";


    game_menu.current = &parent_page;
    game_menu.state = ACTIVE;
}

#endif //__GAME_MENU__