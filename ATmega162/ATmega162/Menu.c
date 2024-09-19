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
	for (uint16_t j = 0; j < 128; j++) {
		char c = pgm_read_byte(&menu->items[j]);
		uint8_t menu_index = j + menu->scroll_offset;
		if (menu_index < menu->num_items){
			for(int i = 0; i < 8; i++){
				SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[(c-32)*8 + i]));
			}
		}
	}
	update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position-menu->scroll_offset);
}
// Oppdater pilene i menyen n�r brukaren navigerer
void update_menu_arrows(uint8_t new_position, uint8_t old_position) {
	// Fjern pila fr� den gamle posisjonen ved � skrive eit mellomrom
	oled_write_char_to_SRAM(old_position, 0, ' ');

	// Set pila p� den nye posisjonen
	oled_write_char_to_SRAM(new_position, 0, '>');
}


// Oppdaterer menyposisjonen fr� joystick-input hugs � kalle p� MultiBoard_Update(board);  // Oppdater joystick-posisjonen
void update_menu_position_from_joystick(MultiBoard* board, Menu* menu) {
	int16_t joyY = (int16_t)(board->JoyYposCal);  // Les Y-posisjonen fr� joysticken

	// Beveg oppover i menyen
	if (joyY > 50) {
		if (menu->current_position > 0) {
			menu->prev_position = menu->current_position;
			menu->current_position--;

			// Hvis vi har scrollet forbi synlege element, oppdater scroll_offset
			// d� m� vi og oppdatere heile menyen
			if (menu->current_position < menu->scroll_offset) {
				menu->scroll_offset--;
				//oled_display_menu(menu);
			} 
			//else{
				//update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position - menu->scroll_offset);
			//}
			_delay_ms(100);  // Forsinkelse for � unng� rask scrolling
		}
	}
	// Beveg nedover i menyen
	else if (joyY < -50) {
		if (menu->current_position < menu->num_items - 1) {
			menu->prev_position = menu->current_position;
			
			menu->current_position++;

			// Hvis vi har n�dd slutten av den synlege menyen, oppdater scroll_offset
			if (menu->current_position >= menu->scroll_offset + MENU_ITEMS_PER_PAGE) {
				menu->scroll_offset++;
			} 
			//else{
				//update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position - menu->scroll_offset);
			//}
			_delay_ms(100);  // Forsinkelse for � unng� rask scrolling
		}
	}
}

// Sjekker om joystick-knappen er trykt
uint8_t is_joystick_button_pressed(MultiBoard* board) {
	return (board->JoyBtn != 0);  // Anta at knappen er aktiv-h�g (1 betyr trykt)
}

void menu_navigate(MultiBoard* board, Menu* menu) {

	// Oppdater joystick- og menyposisjon
	MultiBoard_Update(board);
	update_menu_position_from_joystick(board, menu);
	// Sjekk om knappen er trykt for � bekrefte menyval
	if (is_joystick_button_pressed(board)) {
		handleMenuSelection(board, menu);  // Behandlar menyvalet (trenger tilsyn)
		/*____FEILS�KING________________________
			sjekke att vi kom tilbake hit     */
		oled_write_screen_to_SRAM(solkors);
		oled_data_from_SRAM(); //bytte navn p� dinna funksjonen? d�rlig forklaring av dens funksjon
		_delay_ms(1000); //smile litt
		/*____________________________________*/
		write_menu_oled_to_SRAM(menu);
	}else {
		update_menu_arrows(menu->current_position - menu->scroll_offset,menu->prev_position - menu->scroll_offset);
	}	
}

void write_menu_oled_to_SRAM(Menu* menu){
	// vist m�let er � sjekke om det er meir enn 8 s� blir ditta feil.
	//num_items helder antall "linjer"s� i staden for � sjekke over 128 skulle det vert over 8
	uint8_t menuSize = menu->num_items; 
	
	//Om det er 8 eller mindre linjer (bytte til if(menuSize <= 8){
	if(menuSize <= 128){
		for(uint16_t j = 0; j < menuSize*16; j++) {
			char c = pgm_read_byte(&menu->items[j]);
			for(int i = 0; i < 8; i++){
				if((j%16 == 0) & (j/16 == menu->current_position)){
					SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[('>'-32)*8 + i]));
				}
				else{
					SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[(c-32)*8 + i]));	
				}	
			}	
		}
		// d� blir det vell feil her og ?
		//om menyen er mindre enn 8 linjer fyll resten med ' ' 
		for(uint8_t i = 0; i < 128-menuSize*16; i++){
			for(uint8_t j = 0; j < 8; j++){
				SRAM_write(menuSize*16*8+i*8+j, 0);
			}
		}
	}
	
	
	//Om det er mer enn 8 linjer
	//litt usikker om scroll kan g� for h�y her, men satser p� nei
	else{
		for(uint16_t j = 0; j < menuSize*16; j++) {
			char c = pgm_read_byte(&menu->items[menu->scroll_offset*16 + j]);
			for(int i = 0; i < 8; i++){
				if((j%16 == 0) & (j/16 == menu->current_position)){
					SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[('>'-32)*8 + i]));
				}
				else{
					SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[(c-32)*8 + i]));
				}
			}
		}
	}
	
}




