/*
 * Menu.h
 *
 * Created: 18.09.2024 14:33:01
 *  Author: ravneb
 */ 


#ifndef MENU_H
#define MENU_H
#include "OLED.h"
#define MAX_MENU_ITEMS 5  // For eksempel 5 menyelement

// Deklarer meny og posisjon som eksterne variablar
extern const char* mainMenu[MAX_MENU_ITEMS];
extern uint8_t current_menu_position;

void oled_display_menu(void);
void update_menu_position_from_joystick(MultiBoard* board);
uint8_t is_joystick_button_pressed(MultiBoard* board);
void menu_navigate(MultiBoard* board);

/*Menyer funksjona som nnehelde switchcase*/
void mainMenu_fun(void);

#endif /* MENU_H */