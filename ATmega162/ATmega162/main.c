/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "SRAM.h"
#include "Menu_init.h"

int main(void) {
	/*_________________INITIALISERINGER START______________________*/
	// Initialiser UART med baudrate 9600
	UART_Init(MYUBBR);
	URAT_initStudio();
	UART_EnableReceiveInterrupt();
    setup_timer();						// Start millisekundteljinga
	sei();								// Aktiver globale avbrot
	externalMemoryInit();				// Initialiser eksternt minne må vere etter sei
	initialize_menus();
	MultiBoard board;
	MultiBoard_Init(&board);			// Initialiser MultiBoard og kalibrer joystickens origo
	oled_init();						// Initialiser OLED-skjermen
	//setup_printf_for_oled();			// Optional
	//SRAM_test();
	
	/*______MENY______*/
	current_menu = &mainMenu;
	

	/*_______TEST OLED______________________*/
	
	oled_home();
	write_string_to_SRAM(smiley);
	oled_data_from_SRAM();
	_delay_ms(1000); //smile litt før start
	
	/*_______HOVUDLØKKE______*/
	 while (1) {

        menu_navigate(&board, &current_menu);  // Kallar `menu_navigate` med referanse til gjeldande meny
		
		/*Så lenge vi ikkje har noko delay gåandes og ditta står her tenker eg 
		at den oppdateres automatisk med det minnet vi har skreve til sramen?*/
		if (get_time_in_ms() >= 16) {
			restart_timer();
			oled_data_from_SRAM();
		}

	 }
	return 0;
}
