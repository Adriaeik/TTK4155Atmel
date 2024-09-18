/*
 * OLED.c
 *
 * Created: 18.09.2024 00:10:09
 *  Author: eikel
 */ 

#include "OLED.h"



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

void oled_update_full_screen(uint8_t *data) {
	// Skriv først til dei første 4 sidene (halvparten av skjermen)
	for (uint8_t page = 0; page < 4; page++) {
		oled_set_page(page);              // Velg side (page 0 til 3)
		oled_set_column(0);               // Start frå kolonne 0
		oled_write_data(&data[page * 128], 128);  // Skriv 128 bytes (full breidde) for kvar side
	}

	// Skriv til dei resterande 4 sidene (andre halvdel av skjermen)
	for (uint8_t page = 4; page < 8; page++) {
		oled_set_page(page);              // Velg side (page 4 til 7)
		oled_set_column(0);               // Start frå kolonne 0
		oled_write_data(&data[page * 128], 128);  // Skriv 128 bytes for kvar side
	}
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


void oled_data_to_SRAM(volatile char data[SRAM_OLED_DATA]){ //FUNKER
	for(int i = 0; i<SRAM_OLED_DATA; i++){
		SRAM_write(i, data[i]);
	}
}
void oled_display_SRAM(void) { //TJAAAAA
	for (uint16_t page = 0; page < 8; page++) {  // OLED har 8 sider (pages)
		oled_set_page((page+2)%8);              // Velg aktuell side
		oled_set_column(0);               // Start frå kolonne 0
		oled_write_data(&oled_skjerm_fra_SRAM[page * 128], 128);  // Skriv 128 byte frå SRAM per side
	}
}
void oled_update_display_non_blocking(uint32_t current_time, uint32_t *last_update_time) {
	if ((current_time - *last_update_time) >= 16) {
		oled_display_SRAM();  // Oppdater skjermen frå SRAM
		*last_update_time = current_time;  // Oppdater siste oppdateringstid
	}
}

void write_string_to_SRAM(const char solkors[128]) {
	for (int j = 0; j < 128; j++) {
		// Skriv ut karakteren for å sjå kva vi jobbar med (feilsøking)
		printf("%c", solkors[j]);

		// Ny linje etter 16 karakterar for å formatere utskrift (feilsøking)
		if (j % 16 == 0) {
			printf("\n\r");
		}

		// Forsikre deg om at teiknet er innanfor gyldig ASCII-intervall for fonten din
		if (solkors[j] >= 32 && solkors[j] <= 127) {
			// Skriv teiknet frå fonten til SRAM
			for (uint8_t i = 0; i < 8; i++) {
				SRAM_write(j * 8 + i, pgm_read_byte(&font8x8_basic[(solkors[j] - 32) * 8 + i]));
			}
			} else {
			// Viss teiknet ikkje er gyldig, bruk mellomrom (' ') som standard
			for (uint8_t i = 0; i < 8; i++) {
				SRAM_write(j * 8 + i, pgm_read_byte(&font8x8_basic[(0x20 - 32) * 8 + i])); // ASCII 0x20 for space
			}
		}
	}
}
