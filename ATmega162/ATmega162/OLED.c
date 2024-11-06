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
	if (screen_ms() >= 16) {
		restart_screen_timer();
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
/*
	// Lag en tom buffer fylt med nuller eller mellomrom (0 er nok, fordi funksjonen oversetter til ' ')
	char empty_buffer[128] = {0};  // Heile skjermen, 128 tegn

	// Skriv den tomme bufferen til SRAM, som vil tømme skjermen
	oled_write_screen_to_SRAM(empty_buffer);*/
	for(int i = 0; i < 1024; i++){
		SRAM_write(i, 0);
	}
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

void draw_thick_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	oled_draw_line(x0, y0, x1, y1);           // Hovudlinje
	oled_draw_line(x0 + 1, y0, x1 + 1, y1);   // Skift linje 1 piksel til høgre
	oled_draw_line(x0 - 1, y0, x1 - 1, y1);   // Skift linje 1 piksel til venstre
}

void oled_draw_arc(uint8_t x0, uint8_t y0, uint8_t radius, int start_angle, int end_angle) {
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y) {
		if (start_angle <= 45 && end_angle >= 45) {
			oled_write_pixel_to_SRAM(y0 + y, x0 + x, 1);
		}
		if (start_angle <= 135 && end_angle >= 135) {
			oled_write_pixel_to_SRAM(y0 + x, x0 - y, 1);
		}
		if (start_angle <= 225 && end_angle >= 225) {
			oled_write_pixel_to_SRAM(y0 - y, x0 - x, 1);
		}
		if (start_angle <= 315 && end_angle >= 315) {
			oled_write_pixel_to_SRAM(y0 - x, x0 + y, 1);
		}

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

void draw_number_1() {
	draw_thick_line(72, 16, 72, 48);   // Vertikal linje for talet 1
}

// Liknande funksjonar for andre tal
void draw_number_2() {
	draw_thick_line(72, 16, 104, 16);    // Øvre horisontal linje for 2
	draw_thick_line(104, 16, 104, 32);   // Nedover høgre linje for 2
	draw_thick_line(104, 32, 72, 32);    // Midt horisontal linje for 2
	draw_thick_line(72, 32, 72, 48);     // Venstre nedover linje for 2
	draw_thick_line(72, 48, 104, 48);    // Nedre horisontal linje for 2
}

// Eksempel for talet 3
void draw_number_3() {
	draw_thick_line(72, 16, 104, 16);    // Øvre horisontal linje
	draw_thick_line(104, 16, 104, 48);   // Vertikal høgre linje
	draw_thick_line(72, 32, 104, 32);    // Midtre horisontal linje
	draw_thick_line(72, 48, 104, 48);    // Nedre horisontal linje
}

void draw_number_4() {
	draw_thick_line(72, 16, 72, 32);     // Venstre vertikal linje for talet 4
	draw_thick_line(72, 32, 104, 32);    // Midtre horisontal linje for talet 4
	draw_thick_line(104, 16, 104, 48);   // Høgre vertikal linje for talet 4
}

void draw_number_5() {
	draw_thick_line(104, 16, 72, 16);    // Øvre horisontal linje for talet 5
	draw_thick_line(72, 16, 72, 32);     // Venstre nedover linje for talet 5
	draw_thick_line(72, 32, 104, 32);    // Midtre horisontal linje for talet 5
	draw_thick_line(104, 32, 104, 48);   // Høgre nedover linje for talet 5
	draw_thick_line(72, 48, 104, 48);    // Nedre horisontal linje for talet 5
}

void draw_number_6() {
	draw_thick_line(104, 16, 72, 16);    // Øvre horisontal linje for talet 6
	draw_thick_line(72, 16, 72, 48);     // Venstre vertikal linje for talet 6
	draw_thick_line(72, 48, 104, 48);    // Nedre horisontal linje for talet 6
	draw_thick_line(104, 32, 72, 32);    // Midtre horisontal linje for talet 6
	draw_thick_line(104, 32, 104, 48);   // Høgre nedover linje for talet 6
}

void draw_number_7() {
	draw_thick_line(72, 16, 104, 16);    // Øvre horisontal linje for talet 7
	draw_thick_line(104, 16, 104, 48);   // Høgre nedover linje for talet 7
}

void draw_number_8() {
	draw_thick_line(72, 16, 104, 16);    // Øvre horisontal linje for talet 8
	draw_thick_line(72, 48, 104, 48);    // Nedre horisontal linje for talet 8
	draw_thick_line(72, 16, 72, 48);     // Venstre vertikal linje for talet 8
	draw_thick_line(104, 16, 104, 48);   // Høgre vertikal linje for talet 8
	draw_thick_line(72, 32, 104, 32);    // Midtre horisontal linje for talet 8
}

void draw_number_9() {
	draw_thick_line(72, 16, 104, 16);    // Øvre horisontal linje for talet 9
	draw_thick_line(104, 16, 104, 48);   // Høgre vertikal linje for talet 9
	draw_thick_line(72, 32, 104, 32);    // Midtre horisontal linje for talet 9
	draw_thick_line(72, 16, 72, 32);     // Venstre oppover linje for talet 9
}
void draw_sunset_scene() {
	// Teikne sola
	oled_draw_circle(96, 20, 12);   // Sola

	// Teikne solstrålar med `oled_draw_arc`
	oled_draw_arc(96, 20, 16, 45, 135);    // Øvre venstre stråle
	oled_draw_arc(96, 20, 16, 135, 225);   // Øvre høgre stråle
	oled_draw_arc(96, 20, 16, 225, 315);   // Nedre venstre stråle
	oled_draw_arc(96, 20, 16, 315, 45);    // Nedre høgre stråle

	// Teikne åsen
	oled_draw_line(0, 64, 32, 48);   // Fyrste del av åsen
	oled_draw_line(32, 48, 64, 56);  // Andre del av åsen
	oled_draw_line(64, 56, 96, 40);  // Tredje del av åsen
	oled_draw_line(96, 40, 128, 64); // Fjerde del av åsen, fullfører til høgre kant

	// Teikne innsjøen under åsen
	oled_draw_line(0, 60, 32, 52);   // Første del av innsjølinja
	oled_draw_line(32, 52, 64, 60);  // Andre del av innsjølinja
	oled_draw_line(64, 60, 96, 52);  // Tredje del av innsjølinja
	oled_draw_line(96, 52, 128, 60); // Fjerde del av innsjølinja

	// Teikne eit lite hus på åsen
	oled_draw_square(52, 44, 10, 10);    // Hovudbygget på huset
	oled_draw_line(52, 44, 57, 38);      // Venstre takside
	oled_draw_line(57, 38, 62, 44);      // Høgre takside

	// Teikne eit tre ved sida av huset
	oled_draw_line(70, 44, 70, 38);      // Stammens lengde
	oled_draw_circle(70, 36, 3);         // Topp av treet
}

void draw_pikachu() {
	// Hovudet
	oled_draw_circle(64, 32, 10); // Hovudet er ein sirkel med radius 10

	// Øyre til venstre
	oled_draw_line(58, 22, 54, 12);  // Venstre del av øyret
	oled_draw_line(58, 22, 64, 18);  // Høgre del av venstre øyre

	// Øyre til høgre
	oled_draw_line(70, 22, 74, 12);  // Venstre del av høgre øyre
	oled_draw_line(70, 22, 64, 18);  // Høgre del av høgre øyre

	// Auga
	oled_draw_circle(60, 30, 1);     // Venstre auge
	oled_draw_circle(68, 30, 1);     // Høgre auge

	// Kinn
	oled_draw_circle(55, 35, 2);     // Venstre kinn
	oled_draw_circle(73, 35, 2);     // Høgre kinn

	// Munnen (en enkel smil)
	oled_draw_arc(64, 37, 3, 0, 180); // Nedoverbogen for smil

	// Kroppen
	oled_draw_circle(64, 50, 8);     // Kroppen er ein litt større sirkel

	// Armar
	oled_draw_line(58, 44, 52, 48);  // Venstre arm
	oled_draw_line(70, 44, 76, 48);  // Høgre arm

	// Halen (lynforma)
	oled_draw_line(80, 40, 88, 38);  // Første del av halen oppover
	oled_draw_line(88, 38, 84, 32);  // Nedover del av halen
	oled_draw_line(84, 32, 90, 30);  // Oppover del av halen
}

void draw_sunset_with_pikachu_and_ash() {
	// Teikne sola og solstrålar
	oled_draw_circle(96, 20, 12);   // Sola
	oled_draw_arc(96, 20, 16, 45, 135);    // Øvre venstre stråle
	oled_draw_arc(96, 20, 16, 135, 225);   // Øvre høgre stråle
	oled_draw_arc(96, 20, 16, 225, 315);   // Nedre venstre stråle
	oled_draw_arc(96, 20, 16, 315, 45);    // Nedre høgre stråle

	// Teikne åsen
	oled_draw_line(0, 64, 32, 48);   // Fyrste del av åsen
	oled_draw_line(32, 48, 64, 56);  // Andre del av åsen
	oled_draw_line(64, 56, 96, 40);  // Tredje del av åsen
	oled_draw_line(96, 40, 128, 64); // Fjerde del av åsen, fullfører til høgre kant

	// Teikne innsjøen under åsen
	oled_draw_line(0, 60, 32, 52);   // Første del av innsjølinja
	oled_draw_line(32, 52, 64, 60);  // Andre del av innsjølinja
	oled_draw_line(64, 60, 96, 52);  // Tredje del av innsjølinja
	oled_draw_line(96, 52, 128, 60); // Fjerde del av innsjølinja

	// Tegn Ash på åsen
	oled_draw_circle(50, 35, 3);     // Hovudet til Ash
	oled_draw_line(47, 35, 53, 35);  // Hatten (en enkel linje over hovudet)
	oled_draw_line(50, 38, 50, 45);  // Kroppen til Ash
	oled_draw_line(50, 40, 45, 42);  // Venstre arm
	oled_draw_line(50, 40, 55, 42);  // Høgre arm
	oled_draw_line(50, 45, 47, 50);  // Venstre bein
	oled_draw_line(50, 45, 53, 50);  // Høgre bein

	// Tegn Pikachu ved siden av Ash
	oled_draw_circle(58, 35, 2);     // Hovudet til Pikachu
	oled_draw_line(56, 30, 57, 33);  // Venstre øyre
	oled_draw_line(60, 30, 59, 33);  // Høgre øyre
	oled_draw_circle(56, 36, 1);     // Venstre kinn
	oled_draw_circle(60, 36, 1);     // Høgre kinn
	oled_draw_line(58, 37, 58, 42);  // Kroppen til Pikachu
	oled_draw_line(56, 40, 54, 42);  // Venstre arm
	oled_draw_line(60, 40, 62, 42);  // Høgre arm
	oled_draw_line(58, 42, 56, 46);  // Venstre bein
	oled_draw_line(58, 42, 60, 46);  // Høgre bein

	// Teikne Pikachu sin hale
	oled_draw_line(63, 35, 68, 33);  // Første del av halen
	oled_draw_line(68, 33, 65, 30);  // Andre del av halen
	oled_draw_line(65, 30, 70, 28);  // Tredje del av halen
}
void draw_ash_laughing_normal() {
	// Stort hovud og kropp for Ash
	oled_draw_circle(40, 30, 12);         // Hovudet til Ash
	oled_draw_line(28, 30, 52, 30);       // Hatten (brei linje over hovudet)
	oled_draw_line(40, 42, 40, 60);       // Kroppen til Ash
	oled_draw_line(40, 45, 25, 55);       // Venstre arm, peikar
	oled_draw_line(40, 45, 55, 55);       // Høgre arm

	// Ansikt
	oled_draw_circle(36, 34, 2);          // Venstre auge
	oled_draw_circle(44, 34, 2);          // Høgre auge
	oled_draw_arc(40, 50, 6, 0, 180);     // Stort smil
}

void draw_ash_laughing_hard() {
	// Stort hovud og kropp for Ash med lukka auge og tårer
	oled_draw_circle(40, 30, 12);         // Hovudet til Ash
	oled_draw_line(28, 30, 52, 30);       // Hatten
	oled_draw_line(40, 42, 40, 60);       // Kroppen
	oled_draw_line(40, 45, 25, 55);       // Venstre arm, peikar
	oled_draw_line(40, 45, 55, 55);       // Høgre arm

	// Ansikt med lukka auge og tårer
	oled_draw_line(36, 34, 36, 34);       // Venstre auge lukka
	oled_draw_line(44, 34, 44, 34);       // Høgre auge lukka
	oled_draw_arc(40, 50, 6, 0, 180);     // Stor open munn for latter
	oled_draw_circle(34, 45, 2);          // Tåre på venstre side
	oled_draw_circle(46, 45, 2);          // Tåre på høgre side
}

void draw_pikachu_laughing_normal() {
	// Stort hovud og kropp for Pikachu
	oled_draw_circle(90, 30, 10);         // Hovudet til Pikachu
	oled_draw_line(85, 18, 88, 25);       // Venstre øyre
	oled_draw_line(95, 18, 92, 25);       // Høgre øyre
	oled_draw_circle(86, 36, 2);          // Venstre kinn
	oled_draw_circle(94, 36, 2);          // Høgre kinn
	oled_draw_line(90, 40, 90, 60);       // Kroppen
	oled_draw_line(85, 45, 75, 55);       // Venstre arm, peikar
	oled_draw_line(95, 45, 105, 55);      // Høgre arm

	// Ansikt
	oled_draw_circle(88, 34, 2);          // Venstre auge
	oled_draw_circle(92, 34, 2);          // Høgre auge
	oled_draw_arc(90, 50, 4, 0, 180);     // Stort smil
}

void draw_pikachu_laughing_hard() {
	// Stort hovud og kropp for Pikachu med lukka auge og tårer
	oled_draw_circle(90, 30, 10);         // Hovudet til Pikachu
	oled_draw_line(85, 18, 88, 25);       // Venstre øyre
	oled_draw_line(95, 18, 92, 25);       // Høgre øyre
	oled_draw_circle(86, 36, 2);          // Venstre kinn
	oled_draw_circle(94, 36, 2);          // Høgre kinn
	oled_draw_line(90, 40, 90, 60);       // Kroppen
	oled_draw_line(85, 45, 75, 55);       // Venstre arm, peikar
	oled_draw_line(95, 45, 105, 55);      // Høgre arm

	// Ansikt med lukka auge og tårer
	oled_draw_line(88, 34, 88, 34);       // Venstre auge lukka
	oled_draw_line(92, 34, 92, 34);       // Høgre auge lukka
	oled_draw_arc(90, 50, 5, 0, 180);     // Stor open munn for latter
	oled_draw_circle(84, 46, 2);          // Tåre på venstre side
	oled_draw_circle(96, 46, 2);          // Tåre på høgre side
}


#include <avr/interrupt.h>
#include <avr/wdt.h> // For Watchdog Timer

extern MultiBoard board;
extern volatile uint8_t screen_count;
extern volatile uint8_t screen_count_two = 0;

void animate_ash_and_pikachu_laughing() {
	static uint8_t loooser = 0;
	uint16_t timout = 0;
	// Start Watchdog Timer med 2 sekunders timeout
	//wdt_enable(WDTO_2S);
		oled_clear_screen();
		draw_pikachu_laughing_hard();
		oled_write_line_to_SRAM(0, "!!!DIN TAPAR!!!!");
		oled_data_from_SRAM();
		
	while (board.JoyBtn != 1 && timout < 100) {
		MultiBoard_Update(&board);
		
		if (screen_count == 2) {
			screen_count_two ++;
			
			if (screen_count_two > 102)
			{
				loooser++;
				
				if (loooser >= 347) { loooser = 0; }
			}
		}

		// Teikn utan å clear skjermen kvar gong for å redusere belastning
		if (loooser % 149 == 0) {
			oled_clear_screen();
			oled_write_line_to_SRAM(0, "!!!DIN TAPAR!!!!");
			draw_ash_laughing_normal();
			draw_pikachu_laughing_normal();
			oled_data_from_SRAM();
		}

		if (loooser % 347 == 0) {
			timout++;
			oled_clear_screen();
			oled_write_line_to_SRAM(0, "!!!DIN TAPAR!!!!");
			draw_ash_laughing_hard();
			draw_pikachu_laughing_hard();
			oled_data_from_SRAM();
		}
		printf(" er i while     %d\r", timout);
	}
}

