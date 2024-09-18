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
#include <stdio.h>  // Legg til denne for printf og FILE-støtte


#define MAX_CHARS_PER_LINE 16  // Maks antal teikn per linje

// Variabel for å halde styr på den noverande sida
static uint8_t current_page = 1; 
static uint8_t current_col  = 0;


// Funksjonsdeklarasjonar for OLED-styring MERKA MED ** er nok undvendig, med * kan godt vere men er ubrukt, då vi går vekk fra printf()
void oled_init(void);                          // Initialiserer OLED-skjermen
void oled_set_page(uint8_t page);              // Velg aktiv side (page)
void oled_set_column(uint8_t column);          // Velg kolonne (0-127)
void oled_clear(void);                         //** Tøm skjermen
void oled_clear_page(uint8_t page);            //** Tøm skjermen
void oled_write_data(uint8_t* data, uint16_t size);// Skriv data til skjermen
void oled_write_command(uint8_t command);      // Skriv ein kommando til OLED
void oled_update_full_screen(uint8_t *data);   //** Oppdater heile skjermen med ei blokk med data
void oled_home(void);                          // Gå til første side og kolonne (heimeposisjon)
void oled_print_char(char c);                  // Skriv eit teikn til OLED-skjermen
int oled_putchar(char c, FILE *stream);        // Funksjon for å sende teikn via printf
void setup_printf_for_oled(void);              //* Set opp printf-støtte for OLED
void oled_goto_pos(uint8_t page, uint8_t col); // Sett posisjon for teikn

// SRAM HELVET
char oled_skjerm_fra_SRAM[SRAM_OLED_DATA];

void oled_data_from_SRAM(void);
void oled_update_display_non_blocking(void);
void write_string_to_SRAM(const char solkorset[128]); 

/*______________IKKJE_TESTA______________*/
void oled_write_screen_to_SRAM(const char screen[128]);
void oled_write_FULLscreen_to_SRAM(const char screen_1024[1024]);
void oled_write_line_to_SRAM(uint8_t line, const char* data);
void oled_write_char_to_SRAM(uint8_t row, uint8_t col, char c);
void oled_clear_screen(void);

#endif /* OLED_H_ */
