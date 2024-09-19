/*
 * OLED.c
 *
 * Created: 18.09.2024 00:10:09
 *  Author: eikel
 */ 

#include "OLED.h"
#include <stdlib.h>


// Funksjon for å sende kommando til OLED-skjermen
void oled_write_command(uint8_t command) {
	// Skriv kommandoen til OLED
	Universal_write(OLED_START_IDX, command);
	// ATmega162 håndterer resten (CS, WR) via adresse-dekoding og kontrollsignaler
}
// Funksjon for å sende data til OLED-skjermen
void oled_write_data(uint8_t* data, uint16_t size) {
	// Skriv data til OLED frå startadressa for OLED
	for (uint16_t i = 0; i < size; i++) {
		Universal_write(OLED_START_IDX + 0x200 + i, data[i]); 
	}
}

// Funksjon for å velge aktiv side (page) på OLED-skjermen
void oled_set_page(uint8_t page) {
	oled_write_command(0xB0 | (page & 0x07));  // Velg side (page) sikrar at page-verdien er mellom 0 og 7, sidan OLED-skjermen har 8 sider (page) i vertikal retning (0 til 7). 
}

// Funksjon for å velge kolonne (0-127) på OLED-skjermen
void oled_set_column(uint8_t column) {
	oled_write_command(0x10 | ((column >> 4) & 0x0F));  // Mest signifikante bits
	oled_write_command(0x00 | (column & 0x0F));         // Minst signifikante bits
}

// Funksjon for å tømme skjermen (sette alle pikslar til 0)
void oled_clear(void) {
	oled_goto_pos(1, 0);
	current_page = 1;
	for (uint8_t page = 0; page < 8; page++) {
		oled_set_page(page);
		oled_set_column(0);
		uint8_t empty[128] = {0};  // 128 kolonner med 0 for å tømme sida
		oled_write_data(empty, 128);
	}
}
void oled_clear_page(uint8_t page){
	oled_goto_pos(page, 0);
	uint8_t empty[128] = {0};  // 128 kolonner med 0 for å tømme sida
	oled_write_data(empty, 128);
}

void oled_update_full_screen(uint8_t* buffer) {
	for (uint16_t i = 0; i < OLED_BUFFER_SIZE; i++) {
		SRAM_write(i, buffer[i]);  // Skriv bufferet til SRAM
	}
	oled_data_from_SRAM();  // Oppdater skjermen fra SRAM
}

// Funksjon for å setje OLED-skjermen i heimeposisjon (første side og kolonne)
void oled_home(void) {
	oled_set_page(0);    // Første side
	oled_set_column(0);   // Første kolonne
}

// Funksjon for å initialisere OLED-skjermen
void oled_init(void) {
	// Send relevante kommandoar for å initialisere OLED-skjermen (eksempel)
	oled_write_command(0xae);  // Skru av skjermen
	oled_write_command(0xa1);  // Segment remap
	oled_write_command(0xda);  // Common pads hardware: alternative
	oled_write_command(0x12);  // Common pads alternative setting
	oled_write_command(0xc8);  // Common output scan direction: com63~com0
	oled_write_command(0xa8);  // Multiplex ratio mode
	oled_write_command(0x3f);  // 1/64 duty
	oled_write_command(0xd5);  // Display divide ratio/oscillator frequency mode
	oled_write_command(0x80);  // Set divide ratio/oscillator frequency
	oled_write_command(0x81);  // Contrast control
	oled_write_command(0x50);  // Set contrast to 0x50
	oled_write_command(0xd9);  // Set pre-charge period
	oled_write_command(0x21);  // Set pre-charge period to 0x21
	oled_write_command(0x20);  // Set memory addressing mode
	oled_write_command(0x02);  // Page Addressing Mode
	oled_write_command(0xdb);  // VCOM deselect level mode
	oled_write_command(0x30);  // Set VCOM deselect level
	oled_write_command(0xad);  // Master configuration
	oled_write_command(0x00);  // Select external VCC
	oled_write_command(0xa4);  // Display follows RAM content
	oled_write_command(0xa6);  // Set normal display (not inverted)
	oled_write_command(0xaf);  // Skru på skjermen
}


// Funksjon for å skrive eit teikn til OLED-skjermen frå PROGMEM
void oled_print_char(char c) {
	uint8_t glyph[8];  // Buffer for å lagre teiknets byte frå PROGMEM

	// Hent dei 8 byte som definerer teiknet frå PROGMEM
	for (uint8_t i = 0; i < 8; i++) {
		glyph[i] = pgm_read_byte(&font8x8_basic[(c - 32) * 8 + i]);
	}
	// Skriv teikn-data til OLED-skjermen
	oled_write_data(glyph, 8);  // 8 bytes for kvart teikn
}


// Funksjon for å sende teikn til OLED-skjermen
int oled_putchar(char c, FILE *stream) {
	if (c == '\n') {
		// Flytt til neste side
		current_page++; 
		current_col = 0; 
		// Rull tilbake til første side om vi når siste
		if (current_page > 8) {
			current_page = 1;  // Start på nytt frå side 0
		}
		// Set posisjon til start på den nye sida
		oled_goto_pos(current_page, 0);  // Sett ny side og første kolonne
		} else {
		 // Sjekk om vi har plass til fleire teikn på den noverande linja
		 if (current_col >= MAX_CHARS_PER_LINE) {
			 // Flytt til neste side og nullstill kolonna
			 current_page++;
			 current_col = 0;

			 // Rull tilbake til første side om vi når siste
			 if (current_page >= 8) {
				 current_page = 0;  // Start på nytt frå side 0
			 }

			 oled_goto_pos(current_page, 0);  // Start på ny linje
		 }
		// Skriv teikn til OLED-skjermen
		oled_print_char(c);
		current_col++;
	}

	return 0;
}

// Opprett ein filstrøm knytt til oled_putchar
static FILE oled_stdout = FDEV_SETUP_STREAM(oled_putchar, NULL, _FDEV_SETUP_WRITE);

void setup_printf_for_oled(void) {
	// Set stdout til å bruke OLED som utmatingsstrøm
	stdout = &oled_stdout;
}
// Funksjon for å setje posisjonen til å skrive teikn
void oled_goto_pos(uint8_t page, uint8_t col) {
	oled_set_page(page);   // Velg riktig side (page)
	oled_set_column(col);  // Velg riktig kolonne
}

// SRAM HELVET
void oled_data_from_SRAM(void) {
	for (uint16_t page = 0; page < 8; page++) {
		oled_set_page(page);               // Velg riktig side (page)
		oled_set_column(0);                // Start frå kolonne 0
		uint8_t buffer[128];               // Buffer for å lagre data frå SRAM

		// Les 128 byte frå SRAM for denne sida
		for (uint8_t col = 0; col < 128; col++) {
			buffer[col] = SRAM_read(page * 128 + col);
		}

		// Skriv buffer til OLED-skjermen
		oled_write_data(buffer, 128);
	}
}

void oled_update_display_non_blocking(void) {
	if (get_time_in_ms() >= 16) {
		restart_timer();
		oled_data_from_SRAM();
	}
}


/*______________IKKJE_TESTA______________*/
void oled_write_screen_to_SRAM(const char *screen) {
	for (int j = 0; j < 128; j++) {
		// Les byte direkte frå PROGMEM
		char c = pgm_read_byte(&screen[j]);

		// Forsikre at char er i ASCII-intervall
		if (c >= 32 && c <= 127) {
			// Skriv teiknet frå fonten til SRAM
			for (uint8_t i = 0; i < 8; i++) {
				SRAM_write(j * 8 + i, pgm_read_byte(&font8x8_basic[(c - 32) * 8 + i]));
			}
			} else {
			// Bruk mellomrom (' ') som standard for ugyldige teikn
			for (uint8_t i = 0; i < 8; i++) {
				SRAM_write(j * 8 + i, pgm_read_byte(&font8x8_basic[(0x20 - 32) * 8 + i])); // ASCII 0x20 for space
			}
		}
	}
}


void oled_write_FULLscreen_to_SRAM(const uint8_t *screen_1024) {
	// Gå gjennom hele bufferet og skriv til SRAM, men les frå PROGMEM
	for (uint16_t i = 0; i < 1024; i++) {
		// Les byte direkte frå Flash og skriv til SRAM
		uint8_t byte_from_flash = pgm_read_byte(&screen_1024[i]);
		SRAM_write(i, byte_from_flash);
	}
}


void oled_write_line_to_SRAM(uint8_t line, const char* data) {
	if (line >= 8) {
		return; 
	}
	uint8_t max_chars_per_line = 16;
	uint16_t sram_address = line * 128;  // Hver linje er 128 bytes, 16 karakterer à 8 pixels
	uint8_t i = 0; // så den ikkje døyr når vi går ut av forloopen (brukast igjen)
	// Skriv karakterene til SRAM, men ikke mer enn 16
	for (; i < max_chars_per_line; i++) {
		if (data[i] == '\0') {
			break;  // Avslutt hvis vi når slutten av data
		}
		for (uint8_t j = 0; j < 8; j++) {
			SRAM_write(sram_address + (i * 8) + j, pgm_read_byte(&font8x8_basic[(data[i] - 32) * 8 + j]));
		}
	}
	// Fyll resten av linjen med tomrom (mellomrom) etter slutten av data
	for (; i < max_chars_per_line; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			SRAM_write(sram_address + (i * 8) + j, pgm_read_byte(&font8x8_basic[(' ' - 32) * 8 + j]));
		}
	}
}
//Nyttig for menyen trur eg, lar deg spesifisere nøyaktig kor på skjermen du vil overskrive
void oled_write_char_to_SRAM(uint8_t row, uint8_t col, char c) {
	if (row >= 8 || col >= 16) {
		return; 
	}
	uint16_t sram_address = row * 128 + (col * 8); 
	for (uint8_t i = 0; i < 8; i++) {
		SRAM_write(sram_address + i, pgm_read_byte(&font8x8_basic[(c - 32) * 8 + i]));
	}
}

void oled_write_pixel_to_SRAM(uint8_t row, uint8_t col, uint8_t value) {
	if (row >= 64 || col >= 128) {
		return;
	}
	
	// Finn korrekt byte i SRAM for å justere pikslen
	uint16_t sram_address = (row / 8) * 128 + col;
	uint8_t current_byte = SRAM_read(sram_address);
	uint8_t bit_position = row % 8;

	// Sett eller nullstill den ønskede biten
	if (value) {
		current_byte |= (1 << bit_position);  // Sett bit til 1
		} else {
		current_byte &= ~(1 << bit_position);  // Nullstill bit til 0
	}
	
	SRAM_write(sram_address, current_byte);
}

void oled_clear_screen(void) {
	// Lag en tom buffer fylt med nuller eller mellomrom (0 er nok, fordi funksjonen oversetter til ' ')
	char empty_buffer[128] = {0};  // Heile skjermen, 128 tegn

	// Skriv den tomme bufferen til SRAM, som vil tømme skjermen
	oled_write_screen_to_SRAM(empty_buffer);
}

void oled_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (1) {
		oled_write_pixel_to_SRAM(y0, x0, 1);  // Tegn piksel

		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void oled_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius) {
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y) {
		oled_write_pixel_to_SRAM(y0 + y, x0 + x, 1);
		oled_write_pixel_to_SRAM(y0 + y, x0 - x, 1);
		oled_write_pixel_to_SRAM(y0 - y, x0 + x, 1);
		oled_write_pixel_to_SRAM(y0 - y, x0 - x, 1);
		oled_write_pixel_to_SRAM(y0 + x, x0 + y, 1);
		oled_write_pixel_to_SRAM(y0 + x, x0 - y, 1);
		oled_write_pixel_to_SRAM(y0 - x, x0 + y, 1);
		oled_write_pixel_to_SRAM(y0 - x, x0 - y, 1);

		if (err <= 0) {
			y += 1;
			err += 2 * y + 1;
		}
		if (err > 0) {
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void oled_draw_square(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height) {
	oled_draw_line(x0, y0, x0 + width, y0);           // Øvre linje
	oled_draw_line(x0, y0 + height, x0 + width, y0 + height);  // Nedre linje
	oled_draw_line(x0, y0, x0, y0 + height);           // Venstre linje
	oled_draw_line(x0 + width, y0, x0 + width, y0 + height);   // Høyre linje
}
