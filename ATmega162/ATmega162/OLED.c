/*
 * OLED.c
 *
 * Created: 18.09.2024 00:10:09
 *  Author: eikel
 */ 

#include "OLED.h"



// Funksjon for å sende kommando til OLED-skjermen
void oled_write_command(uint8_t command) {
	// Sett D/!C pinnen til kommandomodus (OLED_CMD_MODE)
	clearBit(OLED_DC_PORT, OLED_DC_PIN);
	// Aktivér Chip Select?
	//clearBit(OLED_CS_PORT, OLED_CS_PIN);
	
	// Send kommandoen til databus
	PORTA = command;  // PortC simulerer databussen her, tilpass til di databuss

//Trur alt ditta fikse seg sjølv
	//// Send skrivingssignal (!WR)
	//clearBit(OLED_WR_PORT, OLED_WR_PIN);
	//_delay_us(1); // Liten forsinkelse for timing
	//clearBit(OLED_WR_PORT, OLED_WR_PIN);
//
//
	//// Deaktiver Chip Select ?
	////OLED_CS_PORT |= (1 << OLED_CS_PIN);
}

// Funksjon for å sende data til OLED-skjermen
void oled_write_data(uint8_t* data, uint16_t size) {
	// Sett D/!C pinnen til datamodus (OLED_DATA_MODE)
	setBit(OLED_DC_PORT, OLED_DC_PIN);

	// Aktivér Chip Select
	clearBit(OLED_CS_PORT, OLED_CS_PIN);

	// Skriv data til skjermen
	for (uint16_t i = 0; i < size; i++) {
		PORTC = data[i];  // Bruk PortC som databuss

		// Send skrivingssignal (!WR)
		clearBit(OLED_WR_PORT, OLED_WR_PIN);
		_delay_us(1); // Liten forsinkelse for timing
		setBit(OLED_WR_PORT, OLED_WR_PIN);
	}

	// Deaktiver Chip Select
	setBit(OLED_CS_PORT, OLED_CS_PIN);
}

// Funksjon for å velge aktiv side (page) på OLED-skjermen
void oled_set_page(uint8_t page) {
	oled_write_command(0xB0 | (page & 0x07));  // Velg side (page)
}

// Funksjon for å velge kolonne (0-127) på OLED-skjermen
void oled_set_column(uint8_t column) {
	oled_write_command(0x10 | ((column >> 4) & 0x0F));  // Mest signifikante bits
	oled_write_command(0x00 | (column & 0x0F));         // Minst signifikante bits
}

// Funksjon for å tømme skjermen (sette alle pikslar til 0)
void oled_clear(void) {
	for (uint8_t page = 0; page < 8; page++) {
		oled_set_page(page);
		oled_set_column(0);
		uint8_t empty[128] = {0};  // 128 kolonner med 0 for å tømme sida
		oled_write_data(empty, 128);
	}
}

// Funksjon for å oppdatere heile skjermen med data
void oled_update_full_screen(uint8_t *data) {
	for (uint8_t page = 0; page < 8; page++) {
		oled_set_page(page);
		oled_set_column(0);
		oled_write_data(&data[page * 128], 128);  // 128 bytes per side
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
	oled_write_command(0xAE);  // Skru av skjermen
	oled_write_command(0xA1);  // Segment remap
	oled_write_command(0xC8);  // COM output scan direction
	oled_write_command(0xA6);  // Normal display (ikkje invertert)
	oled_write_command(0xA4);  // Display follows RAM content
	oled_write_command(0xA8);  // Multiplex ratio
	oled_write_command(0x3F);  // Duty = 1/64
	oled_write_command(0xD3);  // Set display offset
	oled_write_command(0x00);  // No offset
	oled_write_command(0xD5);  // Set display clock divide ratio/oscillator frequency
	oled_write_command(0x80);  // Suggested value
	oled_write_command(0xD9);  // Set pre-charge period
	oled_write_command(0x22);  // Suggested value
	oled_write_command(0xDA);  // Set com pins hardware configuration
	oled_write_command(0x12);  // Suggested value
	oled_write_command(0xDB);  // Set vcomh deselect level
	oled_write_command(0x20);  // Suggested value
	oled_write_command(0x8D);  // Enable charge pump regulator
	oled_write_command(0x14);  // Enable
	oled_write_command(0xAF);  // Skru på skjermen
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
		// Hopp til ny linje på OLED-skjermen (du må definere korleis OLED skal handtere dette)
		oled_goto_pos(/* neste linje */0, 0);  // F.eks. sett ny side og første kolonne
		} else {
		oled_print_char(c);  // Skriv teikn til OLED
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