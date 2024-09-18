/*
 * Menu.c
 *
 * Created: 18.09.2024 14:33:22
 *  Author: ravneb
 */ 
#include "Menu.h"

const char* mainMenu[MAX_MENU_ITEMS] = {
	"Start Game",
	"Settings",
	"High Scores",
	"Credits",
	"Exit"
};
uint8_t current_menu_position = 1;

void oled_display_menu(void) {
	oled_clear();  // Tøm skjermen
	
	// Gå gjennom menyen og vis kvart element
	for (uint8_t i = 0; i < MAX_MENU_ITEMS; i++) {
		oled_goto_pos(i, 0);  // Gå til riktig side/posisjon på skjermen

		// Skriv ut menyvalget, og marker det gjeldande valet
		if (i == current_menu_position) {
			printf("-> %s\n", mainMenu[i]);  // Marker gjeldande posisjon med pil
			} else {
			printf("   %s\n", mainMenu[i]);  // Ingen markering for andre valg
		}
	}
}

void update_menu_position_from_joystick(MultiBoard* board) {
	int16_t joyY = (int16_t)(board->JoyYposCal);  // Les Y-posisjonen frå joysticken

	// Beveg oppover i menyen
	if (joyY > 50) {
		if (current_menu_position > 0) {
			current_menu_position--;
			oled_display_menu();
			_delay_ms(200);  // Liten forsinkelse for å unngå for rask navigering
		}
	}
	// Beveg nedover i menyen
	else if (joyY < -50) {
		if (current_menu_position < MAX_MENU_ITEMS - 1) {
			current_menu_position++;
			oled_display_menu();
			_delay_ms(200);  // Liten forsinkelse for å unngå for rask navigering
		}
	}
}
uint8_t  is_joystick_button_pressed(MultiBoard* board) {
	return (board->JoyBtn != 0) /*|| current_menu_position >3*/;  // Anta at knappen er aktiv-lav (0 betyr trykt)
}
void menu_navigate(MultiBoard* board) {
	oled_display_menu();
	while (1) {
		MultiBoard_Update(board); //Lese verdiane
		update_menu_position_from_joystick(board);

		// Sjekk om knappen er trykt for å bekrefte valg
		if (is_joystick_button_pressed(board)) {
			oled_clear();  // Tøm skjermen
			
			printf("Valgt posisjon: %d (%s)\n", current_menu_position, mainMenu[current_menu_position]);
			break;  // Avslutt løkka når menyvalget er bekrefta
		}
		_delay_ms(100);  // Liten forsinkelse for å redusere prosessorbelastning
	}
}