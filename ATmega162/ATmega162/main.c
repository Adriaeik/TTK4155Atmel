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
		//
	//oled_goto_pos(0, 0);  // Gå til posisjon (side 0, kolonne 0)
	//oled_print_char('A'); // Skriv teiknet 'A'
		//
	//oled_goto_pos(1, 0);  // Gå til neste linje (side 1)
	//oled_print_char('B'); // Skriv teiknet 'B'
	
	oled_init();
	setup_printf_for_oled();
	oled_goto_pos(0, 7);
	printf("Hello, OLED! Page 0\n");
	printf("This is page 1\n");
	printf("This is page 2\n");

	int tall = 0;

	oled_clear();


	while (1) {
		oled_goto_pos(1, 0);
		printf("Hei til OLED    ");
		oled_goto_pos(2, 0);
		printf("                ");
		oled_goto_pos(3, 0);
		printf("Dette burde vere");
		oled_goto_pos(4, 0);
		printf("3/4.linje       ");
		oled_goto_pos(5, 0);
		printf("%d               ", tall);
		if(tall == 10){tall = 0;}
		_delay_ms(200);
		

		
	}

	return 0;
}




