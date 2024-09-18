/*
 * Menu_init.c
 *
 * Created: 18.09.2024 23:12:00
 *  Author: eikel
 */ 
#include "menu_init.h"
#include <avr/pgmspace.h>  // For å bruke PROGMEM

// Definer menyane og lagre dei i flash med PROGMEM
const char mainMenuItems[5][16] PROGMEM = {
	" Start Game   ",
	" Settings     ",
	" High Scores  ",
	" Credits      ",
	" Exit         "
};

const char scrollMenuItems[20][16] PROGMEM = {
	" Item 1       ", " Item 2       ", " Item 3       ", " Item 4       ", " Item 5       ",
	" Item 6       ", " Item 7       ", " Item 8       ", " Item 9       ", " Item 10      ",
	" Item 11      ", " Item 12      ", " Item 13      ", " Item 14      ", " Item 15      ",
	" Item 16      ", " Item 17      ", " Item 18      ", " Item 19      ", " Item 20      "
};

const char settingsMenuItems[4][16] PROGMEM = {
	" Sound        ",
	" Brightness   ",
	" Controls     ",
	" Back         "
};

// Menyobjekt
Menu mainMenu;
Menu scrollMenu;
Menu settingsMenu;

// Funksjon for å lese tekststrenger frå PROGMEM og sette dei inn i menyen
void menu_init_from_flash(Menu* menu, const char menuItems[][16], uint8_t num_items) {
	// Alloker menyarray
	menu->items = malloc(num_items * sizeof(char*));

	// Les menystrenger frå PROGMEM
	for (uint8_t i = 0; i < num_items; i++) {
		menu->items[i] = malloc(16);  // Alloker plass til kvar menystreng
		strncpy_P(menu->items[i], (PGM_P)&menuItems[i], 16);  // Kopier frå flash til RAM
	}

	menu->current_position = 0;
	menu->prev_position = 0;
	menu->scroll_offset = 0;
	menu->num_items = num_items;
}

// Funksjon for å initialisere alle menyane
void initialize_menus() {
	menu_init_from_flash(&mainMenu, mainMenuItems, 5);
	menu_init_from_flash(&scrollMenu, scrollMenuItems, 20);
	menu_init_from_flash(&settingsMenu, settingsMenuItems, 4);
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
			break;
			case 1:
			oled_write_line_to_SRAM(0, "Item 2 valgt");
			break;
			default:
			oled_write_line_to_SRAM(0, "Ugyldig valg");
			break;
		}
		// Når du ønsker å gå tilbake til hovudmenyen, legg inn ein eigen tilbake-funksjon
		// Bruk dette kun når "Back" blir eksplisitt valgt
		break;
	}
}

