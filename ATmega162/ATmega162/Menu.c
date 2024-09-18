/*
 * Menu.c
 *
 * Created: 18.09.2024 14:33:22
 *  Author: ravneb
 */ 
#include "Menu.h"
MenuState currentMenuState = MAIN_MENU;  // Start i hovudmenyen
Menu* current_menu = NULL;

void oled_display_menu(Menu* menu) {
	char buffer[16];  // Buffer for å holde linjene fra PROGMEM

	// Buffer for hele skjermen (128 tegn)
	char screen_buffer[128] = {0};  // Nullstiller hele bufferet

	// Fyll bufferet med menyvalgene (fra PROGMEM)
	for (uint8_t i = 0; i < MENU_ITEMS_PER_PAGE; i++) {
		uint8_t menu_index = i + menu->scroll_offset;
		if (menu_index < menu->num_items) {
			// Kopier menyteksten fra PROGMEM til bufferet
			strncpy_P(buffer, menu->items[menu_index], 16);
			// Kopier linjen til skjermbufferen
			strncpy(&screen_buffer[i * 16], buffer, 16);
		}
	}

	// Skriv bufferet til SRAM
	oled_write_screen_to_SRAM(screen_buffer);

	// Sett pila på gjeldende posisjon innenfor synlig meny
	oled_write_char_to_SRAM(menu->current_position - menu->scroll_offset, 0, '>');
}


// Oppdaterer menyposisjonen frå joystick-input hugs å kalle på MultiBoard_Update(board);  // Oppdater joystick-posisjonen
void update_menu_position_from_joystick(MultiBoard* board, Menu* menu) {
	int16_t joyY = (int16_t)(board->JoyYposCal);  // Les Y-posisjonen frå joysticken

	// Beveg oppover i menyen
	if (joyY > 50) {
		if (menu->current_position > 0) {
			menu->prev_position = menu->current_position;
			menu->current_position--;

			// Hvis vi har scrollet forbi synlege element, oppdater scroll_offset
			if (menu->current_position < menu->scroll_offset) {
				menu->scroll_offset--;
			}
			update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position - menu->scroll_offset);
			_delay_ms(200);  // Forsinkelse for å unngå rask scrolling
		}
	}
	// Beveg nedover i menyen
	else if (joyY < -50) {
		if (menu->current_position < menu->num_items - 1) {
			menu->prev_position = menu->current_position;
			
			menu->current_position++;

			// Hvis vi har nådd slutten av den synlege menyen, oppdater scroll_offset
			if (menu->current_position >= menu->scroll_offset + MENU_ITEMS_PER_PAGE) {
				menu->scroll_offset++;
			}
			update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position - menu->scroll_offset);
			_delay_ms(200);  // Forsinkelse for å unngå rask scrolling
		}
	}
}

// Sjekker om joystick-knappen er trykt
uint8_t is_joystick_button_pressed(MultiBoard* board) {
	return (board->JoyBtn != 0);  // Anta at knappen er aktiv-høg (1 betyr trykt)
}

static uint8_t menu_changed = 1;  // Flag for å spore endringar i menyen
// Funksjon for å navigere i menyen uten blokkering
void menu_navigate(MultiBoard* board, Menu** menu) {

	// Oppdater joystick- og menyposisjon
	MultiBoard_Update(board);
	update_menu_position_from_joystick(board, *menu);

	// Sjekk om menyen har endra seg
	if (menu_changed) {
		oled_display_menu(*menu);  // Oppdater OLED med den nye menyen
		menu_changed = 0;  // Nullstill flagget etter oppdatering
	}

	// Sjekk om knappen er trykt for å bekrefte menyval
	if (is_joystick_button_pressed(board)) {
		oled_clear_screen();
		handleMenuSelection(board, menu);  // Behandlar menyvalet
		menu_changed = 1;  // Menyen vil endre seg etter valet
	}
}

// Oppdater pilene i menyen når brukaren navigerer
void update_menu_arrows(uint8_t new_position, uint8_t old_position) {
	// Fjern pila frå den gamle posisjonen ved å skrive eit mellomrom
	oled_write_char_to_SRAM(old_position, 0, ' ');

	// Set pila på den nye posisjonen
	oled_write_char_to_SRAM(new_position, 0, '>');
}



