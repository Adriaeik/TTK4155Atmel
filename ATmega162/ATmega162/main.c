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
	CAN_Init(MCP2515_MODE_NORMAL);
	
	printf("GO!");
	/*--- Ditta må (noko av det ihvertfall) vere etter sei() ---*/
	externalMemoryInit();				// Initialiser eksternt minne må vere etter sei
	initialize_menus();
	MultiBoard board;
	MultiBoard_Init(&board);			// Initialiser MultiBoard og kalibrer joystickens origo
	oled_init();						// Initialiser OLED-skjermen
		/*--- Optional setup ---*/
			//setup_printf_for_oled();		
			//SRAM_test();
	/*_______OLED + LOGO_______*/
	oled_clear_screen();
	/*- Den komboen her var heilt nydelig -*/
	oled_draw_line(10, 10, 100, 50);
	oled_draw_circle(64, 32, 20);
	oled_draw_square(20, 20, 40, 30);
	oled_data_from_SRAM();
	_delay_ms(500);
			
	/*_________________SPI_________________*/
	// Initialiser SPI og MCP2515 i loopback-modus
	
	
    //MCP2515_SetMode(MCP2515_MODE_CONFIG);  // Sett MCP2515 i Configuration Mode
	// Les CANSTAT-registeret (0x0E) for å sjekke om MCP2515 er i loopback-modus
	uint8_t canstat = MCP2515_Read(0x0f);
	printf("CANSTAT: 0x%X\n", canstat & MCP2515_MODE_MASK);

	if ((canstat & MCP2515_MODE_MASK) == MCP2515_MODE_NORMAL) {  // Loopback-modus har verdi 0x40 i CANSTAT
		printf("MCP2515 er i normal-modus.\n\r");
		} else {
		printf("Feil: MCP2515 er ikke i normal-modus.\n\r");
	}

	sei();								// Aktiver globale avbrot
	
	
	// CAN-melding å sende
	CANMessage msg_to_send = {
		10, // Id
		8, // Lengde på dataen
		"heiiiiii" // Data. Maks åtte byte
	};
	static uint16_t sucsesscout = 0;
	CANMessage received_msg;
	//for (uint16_t i = 0; i < 2047*6; i++)
	//{			
		//
		//msg_to_send.id = 2;
		//msg_to_send.data[2] = i%255;
		//
		//// Send CAN-melding
		//
		//CAN_SendMessage(&msg_to_send);
		//printf("Sendte ID: %d, med den mystiske dataen: %d \n\r", msg_to_send.id, msg_to_send.data[2]);
	//}
	
		
	
	
	
	/*______MENY______*/
	current_menu = &mainMenu; //kan kanskje teste med å starte i ein anna meny
	write_menu_oled_to_SRAM(current_menu);
	
	/*_______HOVUDLØKKE______*/
	 while (1) {

		//if(!CAN_ReceiveMessage(&received_msg)){printf("data[0]: %c adresse = %X\n\r",received_msg.data[0], received_msg.id);}
		//else{printf("Ingen melding tilgjengelig \r\n");}

		//CAN_SendMessage(&msg_to_send);
		
        menu_navigate(&board, current_menu);  // Kallar `menu_navigate` med referanse til gjeldande meny
		
		/*Så lenge vi ikkje har noko delay gåandes og ditta står her tenker eg 
		at den oppdateres automatisk med det minnet vi har skreve til sramen?
		Det kunne vert fornuftig med eit flag her då
		
		16ms skal gi ich 60hz, gitt at resten av programmet kjører raskt nok...
		*/
		
		//if(!CAN_ReceiveMessage(&received_msg)){	printf("data[0]:    %d adresse = %X\n\r",received_msg.data[0], received_msg.id);}
			//
		//if (screen_ms() >= 16) {
			//restart_screen_timer();
			////oled_data_from_SRAM();
		//}
		//if (general_ms() > 65536UL ){ restart_general_timer();}
	 }
	return 0;
}


ISR(INT0_vect) {
	CANMessage msg;
	CAN_ReceiveMessage(&msg);
	//printf("data[0]: %c adresse = %d\n\r",msg.data[0], msg.id);
	MCP2515_BitModify(MCP2515_CANINTF, MCP2515_RX1IF | MCP2515_RX0IF, 0xFF);	
}

ISR(TIMER1_COMPA_vect){
	oled_data_from_SRAM();
};