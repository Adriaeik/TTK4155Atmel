/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "SRAM.h"
#include "OLED.h"

int main(void) {
	// Initialiser UART med baudrate 9600
	UART_Init(MYUBBR);
	
	// Initialiser stdio til � bruke UART
	URAT_initStudio();
	// Aktiver mottaksavbrot
	UART_EnableReceiveInterrupt();
	
	// Aktiver globale avbrot
	sei();
	// Initialiser eksternt minne
	externalMemoryInit();
	// Opprett MultiBoard-struktur for � lagre status
	MultiBoard board;
	// Initialiser MultiBoard og kalibrer joystickens origo
	MultiBoard_Init(&board);
	
	
	//SRAM_test();

//TEST OLED////////////////////////////////////////////
	oled_init();  // Initialiser OLED-skjermen

	setup_printf_for_oled();

	// Vis meny og start navigering
	printf("Velkommen!\n");
	_delay_ms(1000);  // Vent litt f�r du g�r inn i menyen
	oled_clear();
	oled_home();

	// Kall meny-navigeringsfunksjonen
	menu_navigate(&board);

	    // N�r brukaren har valgt eit menyelement, kan du ta handling basert p� valget
	    switch (current_menu_position) {
		    case 0:
			oled_clear();  // T�m skjermen
			oled_home();
		    printf("Startar spelet...\n");
		    // Funksjon for � starte spelet
		    break;
		    case 1:
			oled_clear();  // T�m skjermen
			oled_home();
		    printf("Opnar innstillingar...\n");
		    // Funksjon for � opne innstillingar
		    break;
		    case 2:
			oled_clear();  // T�m skjermen
			oled_home();
		    printf("Viser h�ge poengsummer...\n");
		    // Funksjon for � vise h�gaste poengsummer
		    break;
		    case 3:
			oled_home();
		    printf("Viser kredittar...\n");
		    // Funksjon for � vise kredittar
		    break;
		    case 4:
			oled_clear();  // T�m skjermen
			oled_home();
		    printf("Avsluttar...\n");
		    // Funksjon for � avslutte eller g� ut av menyen
		    break;
		    default:
		    printf("Ugyldig valg\n");
		    break;
	    }
	 // Hovudl�kke
	 while (1) {
		 // Hovudl�kke for andre funksjonar i programmet
	 }



	return 0;
}




	//while (1) {
	////oled_goto_pos(1, 0);
	//printf("Hei til OLED");
	////oled_goto_pos(2, 0);
	//printf("\n");
	////oled_goto_pos(3, 0);
	//printf("Dette burde vere eit reint helvete\n");
	////oled_goto_pos(4, 0);
	//printf("3/4.linje       ");
	////oled_goto_pos(5, 0);
	//printf("%d               ", tall++);
	//printf("                ");
	//printf("                ");
	//if(tall == 10){tall = 0;}
	//_delay_ms(200);
	//}
