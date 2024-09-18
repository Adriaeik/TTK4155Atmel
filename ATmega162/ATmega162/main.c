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
	
	// Initialiser stdio til å bruke UART
	URAT_initStudio();
	// Aktiver mottaksavbrot
	UART_EnableReceiveInterrupt();
	
	// Aktiver globale avbrot
	sei();
	// Initialiser eksternt minne
	externalMemoryInit();
	// Opprett MultiBoard-struktur for å lagre status
	MultiBoard board;
	// Initialiser MultiBoard og kalibrer joystickens origo
	MultiBoard_Init(&board);
	
	
	//SRAM_test();

//TEST OLED////////////////////////////////////////////
	oled_init();  // Initialiser OLED-skjermen

	setup_printf_for_oled();

	// Vis meny og start navigering
	printf("Velkommen!\n");
	_delay_ms(1000);  // Vent litt før du går inn i menyen
	oled_clear();
	oled_home();

	// Kall meny-navigeringsfunksjonen
	menu_navigate(&board);

	    // Når brukaren har valgt eit menyelement, kan du ta handling basert på valget
	    switch (current_menu_position) {
		    case 0:
			oled_clear();  // Tøm skjermen
			oled_home();
		    printf("Startar spelet...\n");
		    // Funksjon for å starte spelet
		    break;
		    case 1:
			oled_clear();  // Tøm skjermen
			oled_home();
		    printf("Opnar innstillingar...\n");
		    // Funksjon for å opne innstillingar
		    break;
		    case 2:
			oled_clear();  // Tøm skjermen
			oled_home();
		    printf("Viser høge poengsummer...\n");
		    // Funksjon for å vise høgaste poengsummer
		    break;
		    case 3:
			oled_home();
		    printf("Viser kredittar...\n");
		    // Funksjon for å vise kredittar
		    break;
		    case 4:
			oled_clear();  // Tøm skjermen
			oled_home();
		    printf("Avsluttar...\n");
		    // Funksjon for å avslutte eller gå ut av menyen
		    break;
		    default:
		    printf("Ugyldig valg\n");
		    break;
	    }
	 // Hovudløkke
	 while (1) {
		 // Hovudløkke for andre funksjonar i programmet
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
