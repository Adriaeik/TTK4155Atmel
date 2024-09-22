/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "SRAM.h"
#include "Menu_init.h"
#include "DriverSPI.h"

int main(void) {
	/*_________________INITIALISERINGER START______________________*/
	/*--- Initialiser UART ---*/
	UART_Init(MYUBBR);
	URAT_initStudio();
	UART_EnableReceiveInterrupt();
    setup_timer();						// Start millisekundteljinga
	sei();								// Aktiver globale avbrot
	/*--- Ditta må (noko av det ihvertfall) vere etter sei() ---*/
	externalMemoryInit();				// Initialiser eksternt minne må vere etter sei
	initialize_menus();
	MultiBoard board;
	MultiBoard_Init(&board);			// Initialiser MultiBoard og kalibrer joystickens origo
	oled_init();						// Initialiser OLED-skjermen
		/*--- Optional setup ---*/
			//setup_printf_for_oled();		
			//SRAM_test();
			
	/*_________________SPI_________________*/
	// Initialiser SPI
	SPI_MasterInit();

	// Test SPI-kommunikasjon
	MCP2515_SendCommand(0xC0);  // Send ein vilkårlig kommando til MCP2515
	uint8_t status = MCP2515_ReadStatus();  // Les status frå MCP2515
	    
	// Sjekk om statusen er korrekt 
	if (status == 0x00) {
		printf("good shit");
		} else {
		printf("bad shit");
	}
		
	/*_______OLED + LOGO_______*/
	oled_clear_screen();
	/*- Den komboen her var heilt nydelig -*/
		oled_draw_line(10, 10, 100, 50);
		oled_draw_circle(64, 32, 20);
		oled_draw_square(20, 20, 40, 30);
	oled_data_from_SRAM();
	_delay_ms(500);
	
	
	/*______MENY______*/
	current_menu = &mainMenu; //kan kanskje teste med å starte i ein anna meny
	write_menu_oled_to_SRAM(current_menu);
	
	/*_______HOVUDLØKKE______*/
	 while (1) {

        menu_navigate(&board, current_menu);  // Kallar `menu_navigate` med referanse til gjeldande meny
		
		/*Så lenge vi ikkje har noko delay gåandes og ditta står her tenker eg 
		at den oppdateres automatisk med det minnet vi har skreve til sramen?
		Det kunne vert fornuftig med eit flag her då
		
		16ms skal gi ich 60hz, gitt at resten av programmet kjører raskt nok...
		*/
		
		
		if (screen_ms() >= 16) {
			restart_screen_timer();
			oled_data_from_SRAM();
		}
		//if (general_ms() > 65536UL ){ restart_general_timer();}
	 }
	return 0;
}
