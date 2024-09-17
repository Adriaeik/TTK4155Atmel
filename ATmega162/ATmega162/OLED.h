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
#include <stdio.h>  // Legg til denne for printf og FILE-st�tte


// Definer pin-funksjonar (tilpass til ditt system)
#define OLED_CMD_MODE 0  // Command mode (D/!C = 0)
#define OLED_DATA_MODE 1 // Data mode (D/!C = 1)


// Eksempeldefinisjonar for kontrollpinnar (tilpass systemet)
#define OLED_CS_PORT PORTB
#define OLED_CS_PIN  PC7   // Chip Select pin
#define OLED_DC_PORT PORTB
#define OLED_DC_PIN  PB3   // Data/Command pin
#define OLED_WR_PORT PORTB
#define OLED_WR_PIN  PB4   // Write pin

// Funksjonsdeklarasjonar for OLED-styring
void oled_init(void);                          // Initialiserer OLED-skjermen
void oled_set_page(uint8_t page);              // Velg aktiv side (page)
void oled_set_column(uint8_t column);          // Velg kolonne (0-127)
void oled_clear(void);                         // T�m skjermen
void oled_write_data(uint8_t* data, uint16_t size);  // Skriv data til skjermen
void oled_write_command(uint8_t command);      // Skriv ein kommando til OLED
void oled_update_full_screen(uint8_t *data);   // Oppdater heile skjermen med ei blokk med data
void oled_home(void);                          // G� til f�rste side og kolonne (heimeposisjon)
void oled_print_char(char c);                  // Skriv eit teikn til OLED-skjermen
int oled_putchar(char c, FILE *stream);        // Funksjon for � sende teikn via printf
void setup_printf_for_oled(void);              // Set opp printf-st�tte for OLED
void oled_goto_pos(uint8_t page, uint8_t col); // Sett posisjon for teikn



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