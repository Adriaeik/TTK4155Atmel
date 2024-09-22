/*
 * Menu_init.c
 *
 * Created: 18.09.2024 23:12:00
 *  Author: eikel
 */ 
#include "menu_init.h"
                




void initialize_menus() {
	/*
	Hadde det vert penare å initialisere main menue her i staden for i main?
	*/
	
	
	mainMenu.items = &mainMenuItems;
	mainMenu.num_items = 5;
	mainMenu.current_position = 0;
	mainMenu.prev_position = 0;
	mainMenu.scroll_offset = 0;

	scrollMenu.items = &scrollMenuItems;
	scrollMenu.num_items = 20;
	scrollMenu.current_position = 0;
	scrollMenu.prev_position = 0;
	scrollMenu.scroll_offset = 0;

	settingsMenu.items = &settingsMenuItems;
	settingsMenu.num_items = 4;
	settingsMenu.current_position = 0;
	settingsMenu.prev_position = 0;
	settingsMenu.scroll_offset = 0;
}


