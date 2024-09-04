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
	//setBit(UCSR0C, UCSZ01);
	//setBit(UCSR0C, UCSZ00);
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
	// Legg til kode for kva som skal gjerast med mottatt data, f.eks. lagre i ein buffer
}
