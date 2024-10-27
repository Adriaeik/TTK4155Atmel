/*
 * Menu.h
 *
 * Created: 18.09.2024 14:33:01
 *  Author: ravneb
 */ 


#ifndef MENU_H
#define MENU_H

#include "OLED.h"
#include "MultiBoard.h" 
#include "GameSettings.h"

#define MENU_ITEMS_PER_PAGE 8  // 8 elementer får plass på skjermen samtidig

/*____Parameter som ein meny trenger____*/
typedef struct {
	char* items;	// Peker til array lagret i PROGMEM
	uint8_t current_position;	// Gjeldande posisjon i menyen
	uint8_t prev_position;		// Forrige posisjon i menyen
	uint8_t scroll_offset;		// Offset for å handtere scrolling
	uint8_t num_items;			// Totalt antall element i menyen
} Menu;

extern Menu mainMenu;
extern Menu scrollMenu;
extern Menu settingsMenu;

/*____Halde kontroll på kva menya vi har____*/
typedef enum {
	MAIN_MENU,
	SCROLL_MENU,
	SETTINGS_MENU
} MenuState;
extern MenuState currentMenuState;  // Start i hovudmenyen
extern Menu* current_menu;  // Global peker til den aktive menyen


// Funksjonsdeklarasjonar for å jobbe med menyar
void oled_display_menu(Menu* menu); // boss - kan fjernast
void write_menu_oled_to_SRAM(Menu* menu);
void update_menu_arrows(uint8_t new_position, uint8_t old_position);
void update_menu_position_from_joystick(MultiBoard* board, Menu* menu);
void menu_navigate(MultiBoard* board, Menu* menu);
/* dinna må fremmover deklareres då den er definert i Menu_init.c
KAnskje det lager noko problem for menyvalg*/
void handleMenuSelection(MultiBoard* board, Menu* menu);		//implementert i menu_init.c


#endif /* MENU_H */
