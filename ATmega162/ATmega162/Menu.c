/*
 * Menu.c
 *
 * Created: 18.09.2024 14:33:22
 *  Author: ravneb
 */ 
#include "Menu.h"
MenuState currentMenuState = MAIN_MENU;  // Start i hovudmenyen
Menu* current_menu = NULL;

// Menyobjekt
Menu mainMenu;
Menu scrollMenu;
Menu settingsMenu;


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
	int32_t deltaT = (general_ms() - board->JoyYLastAction);
	
	board->JoyYLastAction = (deltaT < 0) ? general_ms() : board->JoyYLastAction;
	
	if( deltaT > 100){
		// Beveg oppover i menyen
		if (joyY > 50) {
			if (menu->current_position > 0) {
				menu->prev_position = menu->current_position;
				menu->current_position--;
				// Hvis vi har scrollet forbi synlege element, oppdater scroll_offset
				// då må vi og oppdatere heile menyen
				if (menu->current_position < menu->scroll_offset) {
					menu->scroll_offset--;
				} 
				board->JoyYLastAction = general_ms();  // Forsinkelse for å unngå rask scrolling
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
				board->JoyYLastAction = general_ms();   // Forsinkelse for å unngå rask scrolling
			}
		}
	}
}




void write_menu_oled_to_SRAM(Menu* menu){
	// vist målet er å sjekke om det er meir enn 8 så blir ditta feil.
	//num_items helder antall "linjer"så i staden for å sjekke over 128 skulle det vert over 8
	uint8_t menuSize = menu->num_items; 
	
	//Om det er 8 eller mindre linjer (bytte til if(menuSize <= 8){
	if(menuSize <= 8){
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
		// då blir det vell feil her og ?
		//om menyen er mindre enn 8 linjer fyll resten med ' ' 
		for(uint8_t i = 0; i < 128-menuSize*16; i++){
			for(uint8_t j = 0; j < 8; j++){
				SRAM_write(menuSize*16*8+i*8+j, 0);
			}
		}
	}
	
	
	//Om det er mer enn 8 linjer
	//litt usikker om scroll kan gå for høy her, men satser på nei
	else{
		for(uint16_t j = 0; j < 8*16; j++) {
			char c = pgm_read_byte(&menu->items[menu->scroll_offset*16 + j]);
			for(int i = 0; i < 8; i++){
				if((j%16 == 0) & (j/16 == menu->current_position-menu->scroll_offset)){
					SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[('>'-32)*8 + i]));
				}
				else{
					SRAM_write(j*8 + i, pgm_read_byte(&font8x8_basic[(c-32)*8 + i]));
				}
			}
		}
	}
	
}


void menu_navigate(MultiBoard* board, Menu* menu) {
	// Oppdater joystick- og menyposisjon
	MultiBoard_Update(board);
	update_menu_position_from_joystick(board, menu);
	// Sjekk om knappen er trykt for å bekrefte menyval
	if (board->JoyBtn & !board->JoyBtn_l) {
		handleMenuSelection(board, menu);  // Behandlar menyvalet (trenger tilsyn)
	}
	write_menu_oled_to_SRAM(menu); // 15 ms ikke bra, pontus har ansvar for å finne en løsning
	board->JoyBtn_l = board->JoyBtn;
}


uint8_t playGame;

void handleMenuSelection(MultiBoard* board, Menu* menu) {
	oled_clear_screen();
	switch (currentMenuState) {
		case MAIN_MENU:
		switch (menu->current_position) {
			case 0:
			oled_write_line_to_SRAM(0, "Startar spelet...");
			playGame = 1;
			oled_data_from_SRAM();
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
