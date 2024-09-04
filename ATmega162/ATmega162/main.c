/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"

int main(void) {
	// Initialiser eksternt minne
	externalMemoryInit();

	// Skrive data til SRAM
	SRAM_write(0x1800, 0xAB);  // Skriv 0xAB til adresse 0x1800
	SRAM_write(0x1801, 0xCD);  // Skriv 0xCD til adresse 0x1801

	//// Lese data frå SRAM
	//uint8_t data1 = SRAM_read(0x1800);
	//uint8_t data2 = SRAM_read(0x1801);

	while (1) {
		// Skriv 0x01 til adresse 0x1800, som vil sette PA0 høg og slå på LED kobla til PA0
		SRAM_write(0x1800, 0x01);
		_delay_ms(scaleDelay(500));  // Vent 500 ms

		// Skriv 0x02 til adresse 0x1800, som vil sette PA1 høg og slå på LED kobla til PA1
		SRAM_write(0x1800, 0x02);
		_delay_ms(scaleDelay(500));  // Vent 500 ms

		// Skriv 0x00 til adresse 0x1800, som vil slå av begge LED-ar
		SRAM_write(0x1800, 0x00);
		_delay_ms(scaleDelay(500));  // Vent 500 ms

		// Skriv 0x03 til adresse 0x1800, som vil sette både PA0 og PA1 høg og slå på begge LED-ar
		SRAM_write(0x1800, 0x03);
		_delay_ms(scaleDelay(500));  // Vent 500 ms
	}

	return 0;
}
/*________________HISTORIE_________________________
//int main(void) {
	///*-----------------------
	//Setup start 
	//----------------------*/
	//// Initialiser UART med baudrate 9600
	//UART_Init(MYUBBR);
	//
	//// Initialiser stdio til å bruke UART
	//URAT_initStudio();
	//// Aktiver mottaksavbrot
	//UART_EnableReceiveInterrupt();
	//
	//// Aktiver globale avbrot
	//sei();
	//char buffer[4];  // Buffer for å lagre inndata
    //buffer[0] = 'H';
    //buffer[1] = 'e';
    //buffer[2] = 'i';
    //buffer[3] = '\0';  // Husk nullterminering av strengen
    //
    ///*-----------------------
    //Setup end 
    //----------------------*/
    //printf("BOOTING\n\r");
    //
    //while (1) {
        //// Send ei melding med printf
        //printf("Skriv noko: ");
        //
        //// Send karakterane i bufferet via UART
        //for(int i = 0; i < 3; i++) {
            //UART_SendChar(buffer[i]);
        //}
        //
        //// Skriv det same tilbake
        //printf("\nDu skreiv: %s\n\r", buffer);
        //
        //// Her kan du legge til ekstra logikk, som å sjekke inndata og gjere noko basert på det
        //if (strcmp(buffer, "alder") == 0) {
            //printf("Eg er ein mikrokontroller, eg har ingen alder!\n\r");
        //}
        //
        //// Legg til ei lita forsinkelse (ikkje nødvendig, men for å unngå spam)
        //_delay_ms(scaleDelay(1000));
    //}
	//return 0;
//}
//
//
//int main(void) {
	/*-----------------------
	Setup start 
	----------------------*/
	// Initialiser UART med baudrate 9600
		//// Sett Port B pin 0 som utgang
	//setBit(DDRB, PB0);
	//UART_Init(MYUBBR);
	//
	//// Aktiver mottaksavbrot
	//UART_EnableReceiveInterrupt();
	//
	//// Aktiver globale avbrot
	//sei();
	///*-----------------------
	//Setup end 
	//----------------------*/
	//
	//while (1) {
		//// Send ein testkarakter
		//UART_SendChar('A');
		//UART_SendChar('z');
		//UART_SendChar('\n');
		//UART_SendChar('\r');
		//
		////Sett PB0 høg
		//setBit(PORTB, PB0);
		//// Legg til forsinkelse
		////_delay_ms(scaleDelay(1000));
		//UART_ReceiveChar();
		////Sett PB0 låg
		//clearBit(PORTB, PB0);
	//}
	//
	//return 0;
//}

//int main(void) {
	//// Sett Port B pin 0 som utgang
	////DDRB |= (1 << PB0);
	//setBit(DDRB, PB0);
//
	//while (1) {
		//// Sett PB0 høg
		////PORTB |= (1 << PB0);
		//setBit(PORTB, PB0);
		//_delay_ms(500);  // Vent 500 ms
//
		//// Sett PB0 låg
		////PORTB &= ~(1 << PB0);
		//clearBit(PORTB, PB0);
		//_delay_ms(500);  // Vent 500 ms
	//}
//
	//return 0;
//}
