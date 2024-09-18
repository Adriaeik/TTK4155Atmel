/*
 * OLED.h
 *
 * Created: 18.09.2024 00:09:21
 *  Author: eikel
 */ 


#ifndef OLED_H_
#define OLED_H_
#include "Utils.h"
#include "Font.h" // for PROGMEM og pgm_read_byte
#include "XMEM_Decode.h"
#include "MultiBoard.h"
#include <stdio.h>  // Legg til denne for printf og FILE-st�tte


// Definer pin-funksjonar (tilpass til ditt system)
#define OLED_CMD_MODE 0  // Command mode (D/!C = 0)
#define OLED_DATA_MODE 1 // Data mode (D/!C = 1)
#define MAX_CHARS_PER_LINE 16  // Maks antal teikn per linje

// Eksempeldefinisjonar for kontrollpinnar (tilpass systemet)
#define OLED_WR_PORT PORTB
#define OLED_WR_PIN  PB4  




// Variabel for � halde styr p� den noverande sida
static uint8_t current_page = 1; 
static uint8_t current_col  = 0;


// Funksjonsdeklarasjonar for OLED-styring
void oled_init(void);                          // Initialiserer OLED-skjermen
void oled_set_page(uint8_t page);              // Velg aktiv side (page)
void oled_set_column(uint8_t column);          // Velg kolonne (0-127)
void oled_clear(void);                         // T�m skjermen
void oled_clear_page(uint8_t page);                        // T�m skjermen
void oled_write_data(uint8_t* data, uint16_t size);  // Skriv data til skjermen
void oled_write_command(uint8_t command);      // Skriv ein kommando til OLED
void oled_update_full_screen(uint8_t *data);   // Oppdater heile skjermen med ei blokk med data
void oled_home(void);                          // G� til f�rste side og kolonne (heimeposisjon)
void oled_print_char(char c);                  // Skriv eit teikn til OLED-skjermen
int oled_putchar(char c, FILE *stream);        // Funksjon for � sende teikn via printf
void setup_printf_for_oled(void);              // Set opp printf-st�tte for OLED
void oled_goto_pos(uint8_t page, uint8_t col); // Sett posisjon for teikn

// SRAM HELVET
char oled_skjerm_fra_SRAM[SRAM_OLED_DATA];

void oled_data_from_SRAM(void);
void oled_data_to_SRAM(volatile char data[SRAM_OLED_DATA]);
void oled_display_SRAM(void);
void oled_update_display_non_blocking(uint32_t current_time, uint32_t *last_update_time);
void write_string_to_SRAM(const char solkors[128]); 
#endif /* OLED_H_ */

/* main eksempel*/
//int main(void) {
	//oled_init();  // Initialiser OLED-skjermen
	//
	//oled_goto_pos(0, 0);  // G� til posisjon (side 0, kolonne 0)
	//oled_print_char('A'); // Skriv teiknet 'A'
//
	//oled_goto_pos(1, 0);  // G� til neste linje (side 1)
	//oled_print_char('B'); // Skriv teiknet 'B'
//
	//while (1) {
		//// Hovudl�kka
	//}
	//
	//return 0;
//}

/*
int main(void) {
	oled_init();             
	setup_printf_for_oled();  
	oled_goto_pos(0, 0);      
	printf("Hello, OLED! Page 0\n"); 
	printf("This is page 1\n"); 
	printf("This is page 2\n"); 
	printf("This is page 3\n"); 
	printf("This is page 4\n"); 
	printf("This is page 5\n");
	printf("This is page 6\n");
	printf("This is page 7\n");
  

	while (1) {
		
	}

	return 0;
}
*/
