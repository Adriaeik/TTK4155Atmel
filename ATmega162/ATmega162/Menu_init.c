/*
 * Menu_init.c
 *
 * Created: 18.09.2024 23:12:00
 *  Author: eikel
 */ 
#include "menu_init.h"
                


// Menyobjekt
Menu mainMenu;
Menu scrollMenu;
Menu settingsMenu;

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

void handleMenuSelection(MultiBoard* board, Menu* menu) {
	switch (currentMenuState) {
		case MAIN_MENU:
		switch (menu->current_position) {
			case 0:
			oled_write_line_to_SRAM(0, "Startar spelet...");
			break;
			case 1:
			// Gå til innstillingar
			currentMenuState = SETTINGS_MENU;
			current_menu = &settingsMenu;  // Oppdater til innstillingsmeny
			break;
			case 2:
			// Gå til scrollemeny
			currentMenuState = SCROLL_MENU;
			current_menu = &scrollMenu;  // Oppdater til scrollemeny
			break;
			case 3:
			oled_write_line_to_SRAM(0, "Viser kredittar...");
			oled_write_screen_to_SRAM(solkors);  // Vis kredittar på skjermen
			break;
			case 4:
			oled_write_line_to_SRAM(0, "Avsluttar...");
			// Legg eventuelt til funksjonalitet for å avslutte
			break;
			default:
			oled_write_line_to_SRAM(0, "Ugyldig valg");
			break;
		}
		break;

		case SETTINGS_MENU:
		// Håndter valg i innstillingsmenyen
		switch (menu->current_position) {
			case 0:
			oled_write_line_to_SRAM(0, "Endrer lydinnstillingar...");
			break;
			case 1:
			oled_write_line_to_SRAM(0, "Endrer lysstyrke...");
			break;
			case 2:
			oled_write_line_to_SRAM(0, "Endrer kontrollar...");
			break;
			case 3:
			// Gå tilbake til hovudmenyen
			currentMenuState = MAIN_MENU;
			current_menu = &mainMenu;
			break;
			default:
			oled_write_line_to_SRAM(0, "Ugyldig valg");
			break;
		}
		break;

		case SCROLL_MENU:
		// Håndter valg i scrollemenyen
		switch (menu->current_position) {
			case 0:
			oled_write_line_to_SRAM(0, "Item 1 valgt");
			currentMenuState = MAIN_MENU;
			current_menu = &mainMenu;
			break;
			case 1:
			oled_write_line_to_SRAM(0, "Item 2 valgt");
			break;
			default:
			oled_write_line_to_SRAM(0, "Ugyldig valg");
			break;
		}
		break;
	}
}

