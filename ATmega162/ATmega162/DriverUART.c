#include "DriverUART.h"
#include "utils.h"  // Inkluder utils.h for å bruke makroane derifrå
#include <avr/interrupt.h>  // For sei() og avbrotsvektorar

// Initialiser UART med gitt baudrate
void UART_Init(unsigned int ubrr) {
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable receiver and transmitter
	setBit(UCSR0B, RXEN0);  // Aktivere mottakar
	setBit(UCSR0B, TXEN0);  // Aktivere sendar
	
	// Set frame format: 8 data bits, 2 stop bit
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);

}

// Send eit teikn via UART
void UART_SendChar(char data) {
	// Vent til sendebufferet er klart
	loopUntilBitIsSet(UCSR0A, UDRE0);
	
	// Send data
	UDR0 = data;
}

// Motta eit teikn via UART
char UART_ReceiveChar(void) {
	// Vent til data er mottatt
	loopUntilBitIsSet(UCSR0A, RXC0);
	
	// Returner mottatt data
	return UDR0;
}
// Funksjon for å sende ein karakter via UART
int UART_putChar(char c, FILE *stream) {
	UART_SendChar(c);
	return 0;
}
// Funksjon for å motta ein karakter via UART
int UART_getChar(FILE *stream) {
	char c = UART_ReceiveChar();
	UART_SendChar(c);  // Echo tilbake til terminalen slik at du ser kva du skriv
	return c;
}
//void URAT_initStudio() {
	////// Koble UART_putchar til printf (stdout)
	//fdevopen(UART_SendChar, UART_ReceiveChar);
//}

void URAT_initStudio() {
	// Definer ein FILE struktur som knytter vår UART_putchar til stdout og UART_getchar til stdin
	static FILE uart_stdout = FDEV_SETUP_STREAM(UART_putChar, NULL, _FDEV_SETUP_WRITE);
	static FILE uart_stdin  = FDEV_SETUP_STREAM(NULL, UART_getChar, _FDEV_SETUP_READ);
	
	// Sett stdout og stdin til å bruke våre FILE strukturer
	stdout = &uart_stdout;
	stdin  = &uart_stdin;
}
// Aktiver avbrot for når data blir mottatt
void UART_EnableReceiveInterrupt(void) {
	setBit(UCSR0B, RXCIE0);
}

// Deaktiver avbrot for når data blir mottatt
void UART_DisableReceiveInterrupt(void) {
	clearBit(UCSR0B, RXCIE0);
}

// UART mottaksavbrotvektor USART0_RX_vect
ISR(USART0_RXC_vect) {
	// Handter mottatt data
	char received = UDR0;
}
