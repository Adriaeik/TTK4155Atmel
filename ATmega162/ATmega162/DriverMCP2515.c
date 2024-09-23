/*
 * DriverMCP2515.c
 *
 * Created: 22.09.2024 13:56:38
 *  Author: eikel
 */ 
#include "DriverMCP2515.h"

void set_ss(void){
	setBit(PORTB, PB4);	
}

void clear_ss(void){
	clearBit(PORTB, PB4);
}



// Init kokt rett fra Waseem
void MCP2515_init(void) {
	SPI_Init();
	MCP2515_Reset(); //Se kode for denne lenger ned

	_delay_ms(1); //Viktig!

	// Sjøltesting
	uint8_t value = MCP2515_Read(MCP2515_CANSTAT); 
	if ((value & MCP2515_MODEMASK) != MCP2515_MODE_CONFIG) { //endret denne
		printf("MCP2515 er ikkje i konfigurasjonsmodus etter reset. CANSTAT: %x \r\n", value);
	}
}
// MCP2515 Reset-funksjon
void MCP2515_Reset(void) {
	clear_ss();
	SPI_Transmit(MCP2515_RESET);  // Send RESET-kommandoen
	set_ss();
}

// MCP2515 Read-funksjon
uint8_t MCP2515_Read(uint8_t address) {
	uint8_t result;

	clear_ss();	               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_READ);         // Send READ-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil lese fra
	result = SPI_Receive();             // Les resultatet fra MCP2515
	set_ss();                 // Deaktiver slave (CS h�y)

	return result;
}

// MCP2515 Write-funksjon
void MCP2515_Write(uint8_t address, uint8_t data) {
	clear_ss();			               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_WRITE);        // Send WRITE-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil skrive til
	SPI_Transmit(data);                 // Skriv dataen til MCP2515
	set_ss();			                // Deaktiver slave (CS h�y)
}

// MCP2515 Request to Send-funksjon
void MCP2515_RequestToSend(uint8_t txBuffers) {
	clear_ss();		               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_RTS_TX0 | txBuffers);  // Send RTS-kommandoen med buffer-parameter
	set_ss();		                 // Deaktiver slave (CS h�y)
}
// KOK (ifølge chat er vår lik)
// void mcp_request_to_send(int buffer_number) {
// 	spi_clear_ss();
// 	buffer_number = buffer_number % 3; // Mapper buffernummer til 0, 1, 2
// 	char data = MCP_RTS_TX0;
// 	if (buffer_number == 0) {
// 		data = MCP_RTS_TX0;
// 	} else if (buffer_number == 1) {
// 		data = MCP_RTS_TX1;
// 	} else if (buffer_number == 2) {
// 		data = MCP_RTS_TX2;
// 	}
// 	spi_write(data);
// 	spi_set_ss();
// }

// MCP2515 Read Status-funksjon
uint8_t MCP2515_ReadStatus(void) {
	uint8_t status;

	clear_ss();			               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_READ_STATUS);  // Send READ STATUS-kommandoen
	status = SPI_Receive();             // Les status
	set_ss();			                 // Deaktiver slave (CS h�y)

	return status;
}

// MCP2515 Bit Modify-funksjon
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
	clear_ss();			               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_BIT_MODIFY);   // Send BIT MODIFY-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil modifisere
	SPI_Transmit(mask);                 // Send bitmasken
	SPI_Transmit(data);                 // Skriv nye bit-verdier
	set_ss();			                 // Deaktiver slave (CS h�y)
}

//// MCP2515 Send Command-funksjon
//void MCP2515_SendCommand(uint8_t command) {
	//clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	//SPI_Transmit(command);              // Send kommandoen til MCP2515
	//setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)
//}

// MCP2515 Set Mode-funksjon
void MCP2515_SetMode(uint8_t mode) {
	MCP2515_BitModify(MCP2515_CANCTRL, MCP2515_CANSTAT, mode);  // Sett mode-bits (bits 5-7) i CANCTRL-registeret
	
	//Legger til error sjekk her:
	uint8_t newMode = MCP2515_Read(MCP2515_CANSTAT);
	if((newMode | MCP2515_MODEMASK) != mode){
		printf("ERROR: MCP2515_SetMode satt ikke mode riktig");
	}
}
