/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "SRAM.h"
#include "Menu_init.h"
#include "DriverCAN.h"

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
	// Initialiser SPI og MCP2515 i loopback-modus
	SPI_Init();
	CAN_Init();

	// CAN-melding å sende
	CANMessage msg_to_send;
	msg_to_send.id = 0x123;  // Eksempel-ID
	msg_to_send.length = 3;
	msg_to_send.data[0] = 0x11;
	msg_to_send.data[1] = 0x22;
	msg_to_send.data[2] = 0x33;

	// Send CAN-melding
	CAN_SendMessage(&msg_to_send);

	_delay_ms(100);  // Vent litt før vi prøver å lese meldingen (simuler en liten forsinkelse)

	// Mottak CAN-melding
	CANMessage received_msg = CAN_ReceiveMessage();

	// Sjekk om mottatt melding er lik som den sendte
	if (received_msg.id == msg_to_send.id && received_msg.length == msg_to_send.length) {
		uint8_t matching_data = 1;
		for (uint8_t i = 0; i < received_msg.length; i++) {
			if (received_msg.data[i] != msg_to_send.data[i]) {
				matching_data = 0;
				break;
			}
		}

		if (matching_data) {
			// Meldingene samsvarer - loopback-testen er vellykket
			printf("Loopback test successful! Received ID: 0x%X, Data: %X %X %X\n", received_msg.id, received_msg.data[0], received_msg.data[1], received_msg.data[2]);
			} else {
			// Dataene samsvarer ikke
			printf("Loopback test failed! Data mismatch.\n");
		}
		} else {
		// ID eller lengde samsvarer ikke
		printf("Loopback test failed! ID or length mismatch.\n");
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
