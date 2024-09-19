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
	// Buffer for å holde linjene fra PROGMEM og Buffer for hele skjermen (128 tegn)
	char buffer[16];  
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
	// Skriv bufferet til SRAM og Sett pila på gjeldende posisjon innenfor synlig meny
	oled_write_screen_to_SRAM(screen_buffer);
	oled_write_char_to_SRAM(menu->current_position - menu->scroll_offset, 0, '>');
}
// Oppdater pilene i menyen når brukaren navigerer
void update_menu_arrows(uint8_t new_position, uint8_t old_position) {
	// Fjern pila frå den gamle posisjonen ved å skrive eit mellomrom
	oled_write_char_to_SRAM(old_position, 0, ' ');

	// Set pila på den nye posisjonen
	oled_write_char_to_SRAM(new_position, 0, '>');
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
			// då må vi og oppdatere heile menyen
			if (menu->current_position < menu->scroll_offset) {
				menu->scroll_offset--;
				//oled_display_menu(menu);
			} 
			//else{
				//update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position - menu->scroll_offset);
			//}
			_delay_ms(100);  // Forsinkelse for å unngå rask scrolling
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
			//else{
				//update_menu_arrows(menu->current_position - menu->scroll_offset, menu->prev_position - menu->scroll_offset);
			//}
			_delay_ms(100);  // Forsinkelse for å unngå rask scrolling
		}
	}
}

// Sjekker om joystick-knappen er trykt
uint8_t is_joystick_button_pressed(MultiBoard* board) {
	return (board->JoyBtn != 0);  // Anta at knappen er aktiv-høg (1 betyr trykt)
}

static uint8_t menu_changed = 1;  // Flag for å spore endringar i menyen
// Funksjon for å navigere i menyen uten blokkering

/*
Grunnen til at vi bruker en dobbeltpeker (Menu**) i menu_navigate er at det 
tillater oss å modifisere pekeren som peker til den aktive menyen 
(f.eks. current_menu) inne i funksjonen. Dette er nyttig hvis vi trenger 
å bytte meny underveis, f.eks. fra hovedmeny til innstillingsmeny.
- Ganske fett
*/
void menu_navigate(MultiBoard* board, Menu* menu) {

	// Oppdater joystick- og menyposisjon
	MultiBoard_Update(board);
	update_menu_position_from_joystick(board, menu);



	// Sjekk om knappen er trykt for å bekrefte menyval
	if (is_joystick_button_pressed(board)) {
		
		handleMenuSelection(board, menu);  // Behandlar menyvalet
		
	}
	write_menu_oled_to_SRAM(menu);
	
	
}

void write_menu_oled_to_SRAM(Menu* menu){
	uint8_t menuSize = menu->num_items;
	
	//Om det er 8 eller mindre linjer
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
		//om menyen er mindre enn 8 linjer fyll resten med ' ' 
		for(uint8_t i = 0; i < 128-menuSize*16; i++){
			for(uint8_t j = 0; j < 8; j++){
				SRAM_write(menuSize*16+i*16+j, 0);
			}
		}
	}
	
	
	//Om det er mer enn 8 linjer
	//litt usikker om scroll kan gå for høy her, men satser på nei
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




