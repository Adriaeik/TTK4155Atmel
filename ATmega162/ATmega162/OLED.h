/*
 * OLED.h
 *
 * Created: 18.09.2024 00:09:21
 *  Author: eikel
 */ 
/* VI KAN NOK FJERNE HALVPARTEN HER :) */

#ifndef OLED_H_
#define OLED_H_
#include "Utils.h"
/*
#include "Font.h" // for PROGMEM og pgm_read_byte
*/
#include "XMEM_Decode.h"
#include "MultiBoard.h"
#include <stdio.h>  // Legg til denne for printf og FILE-støtte


#define MAX_CHARS_PER_LINE 16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 8)  // 1024 bytes


// Variabel for å halde styr på den noverande sida
static uint8_t current_page = 1; 
static uint8_t current_col  = 0;


// Funksjonsdeklarasjonar for OLED-styring Merka med ¤ skriver ikkje via RAM (rett på displayet)
// merka med `f` er for printf, MERKA MED ** er nok undvendig, med * kan godt vere men er ubrukt, då vi går vekk fra printf()
void oled_init(void);                          // Initialiserer OLED-skjermen
void oled_set_page(uint8_t page);              // Velg aktiv side (page)
void oled_set_column(uint8_t column);          // Velg kolonne (0-127)
void oled_write_data(uint8_t* data, uint16_t size);// Skriv data til skjermen
void oled_write_command(uint8_t command);      // Skriv ein kommando til OLED
void oled_goto_pos(uint8_t page, uint8_t col); // Sett posisjon for teikn

/*______TRUR DITTA KAN FJERNAST______*/
//eigentlig overflod
void oled_clear(void);                         //¤ ** Tøm skjermen
void oled_clear_page(uint8_t page);            //¤ ** Tøm skjermen
void oled_update_full_screen(uint8_t *data);   //** Oppdater heile skjermen med ei blokk med data
void oled_home(void);                          //* Gå til første side og kolonne (heimeposisjon)
void oled_print_char(char c);                  //`f` ¤ Skriv eit teikn til OLED-skjermen
int oled_putchar(char c, FILE *stream);        //`f` Funksjon for å sende teikn via printf
void setup_printf_for_oled(void);              //`f` * Set opp printf-støtte for OLED
/*___________________________________*/

// SRAM HELVET  - merka med ¤ trur eg er overflod og unødvendige funksjona
void oled_data_from_SRAM(void);
void oled_update_display_non_blocking(void);				// ¤ bruker non blocking opdatering, men kanskje like greit å ha dinna if setninga på utsida

/*______________IKKJE_TESTA______________*/
void oled_write_screen_to_SRAM(const char *screen);					//sender inn ein array av char som dekker skjermen, skal fynne tomme rom med space (` `)
void oled_write_FULLscreen_to_SRAM(const uint8_t *screen_1024);		//sender inn ein bitstring
void oled_write_line_to_SRAM(uint8_t line, const char* data);			//lar deg skrive på kun ein linje, overskriver alt på linja
void oled_write_char_to_SRAM(uint8_t row, uint8_t col, char c);			//lar deg overskrive ein spesifik char på displayet, og kun det. slik at man kan enkelt flytte menypila uten å skrive heile skjermen
void oled_write_pixel_to_SRAM(uint8_t row, uint8_t col, uint8_t value);	//lar deg teikne ein pixel på spesifik plass, brukt av teikningfunksjonane
void oled_clear_screen(void);											//bruker oled_write_screen_to_SRAM og sender ein tom buffer slik at skjermen blir blank


/*ARKITEKTUR*/
void oled_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void oled_circle(uint8_t x, uint8_t y, uint8_t r, uint8_t* buffer);
void oled_draw_square(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
#endif /* OLED_H_ */
