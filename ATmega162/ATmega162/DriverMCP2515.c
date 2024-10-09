/*
 * DriverMCP2515.c
 *
 * Created: 22.09.2024 13:56:38
 *  Author: eikel
 */ 
#include "DriverMCP2515.h"

// Init kokt rett fra Waseem
void MCP2515_init() {
	SPI_MasterInit();
	//Kommer hit
	MCP2515_Reset(); //Se kode for denne lenger ned
	_delay_ms(4); //Viktig!
	uint8_t value = MCP2515_Read(MCP2515_CANSTAT);
	if ((value & MCP2515_MODE_MASK) != MCP2515_MODE_CONFIG) {
		printf("MCP2515 er ikkje i konfigurasjonsmodus etter reset. CANSTAT: %x \r\n", value);
		}
		
		
	/*___________CANBUS bit timing______________*/
	uint8_t BRP = FOSC/ (20 * 125000);
	MCP2515_Write(MCP2515_CNF1, MCP2515_SJW4 | (BRP-1));
	MCP2515_Write(MCP2515_CNF2, MCP2515_BTLMODE | MCP2515_SAMPLE_1X | ((3-1) << 3) | (2-1));
	MCP2515_Write(MCP2515_CNF3, MCP2515_WAKFIL_DISABLE | (4 - 1));
}
// MCP2515 Reset-funksjon
void MCP2515_Reset(void) {
	SPI_SelctSlave();
	MCP2515_SendCommand(MCP2515_RESET);  // Send RESET-kommandoen
	SPI_DselectSlave();
}

// MCP2515 Read-funksjon
uint8_t MCP2515_Read(uint8_t address) {
	uint8_t result= 0xFF; 
	SPI_SelctSlave();               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_READ_);         // Send READ-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil lese fra
	result = SPI_Receive();             // Les resultatet fra MCP2515
	SPI_DselectSlave();                 // Deaktiver slave (CS h�y)

	return result;
}

// MCP2515 Write-funksjon
void MCP2515_Write(uint8_t address, uint8_t data) {
	SPI_SelctSlave();               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_WRITE);        // Send WRITE-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil skrive til
	SPI_Transmit(data);                 // Skriv dataen til MCP2515
	SPI_DselectSlave();                 // Deaktiver slave (CS h�y)
}

//void MCP2515_RequestToSend(uint8_t txBuffers) {
	//if (txBuffers > 2) {
		//// Feil buffer, returner ingenting
		//return;
	//}
//
	//SPI_SelctSlave();  // Aktiver slave (CS lav)
	//
	//// Send RTS-kommando for spesifisert buffer
	//switch (txBuffers) {
		//case 0:
		//SPI_Transmit(MCP2515_RTS_TX0);  // Send RTS for TXB0
		//break;
		//case 1:
		//SPI_Transmit(MCP2515_RTS_TX1);  // Send RTS for TXB1
		//break;
		//case 2:
		//SPI_Transmit(MCP2515_RTS_TX2);  // Send RTS for TXB2
		//break;
	//}
//
	//SPI_DselectSlave();  // Deaktiver slave (CS høy)
//}
// MCP2515 Request to Send-funksjon
void MCP2515_RequestToSend(uint8_t txBuffers) {
	if (txBuffers > 2) {
		// Feilverdi
		printf("feil i MCP2515_RequestToSend");
		return; 
	}
	SPI_SelctSlave();               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_RTS_TX0 | txBuffers);  // Send RTS-kommandoen med buffer-parameter
	SPI_DselectSlave();                 // Deaktiver slave (CS h�y)
}

// MCP2515 Read Status-funksjon
uint8_t MCP2515_ReadStatus(void) {
	uint8_t status;

	SPI_SelctSlave();               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_READ_STATUS);  // Send READ STATUS-kommandoen
	status = SPI_Receive();             // Les status
	SPI_DselectSlave();                 // Deaktiver slave (CS h�y)

	return status;
}

// MCP2515 Bit Modify-funksjon
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
	SPI_SelctSlave();               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_BIT_MODIFY);   // Send BIT MODIFY-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil modifisere
	SPI_Transmit(mask);                 // Send bitmasken
	SPI_Transmit(data);                 // Skriv nye bit-verdier
	SPI_DselectSlave();                 // Deaktiver slave (CS h�y)
}

// MCP2515 Send Command-funksjon
void MCP2515_SendCommand(uint8_t command) {
	SPI_SelctSlave();               // Aktiver slave (CS lav)
	SPI_Transmit(command);              // Send kommandoen til MCP2515
	SPI_DselectSlave();                 // Deaktiver slave (CS h�y)
}

// MCP2515 Set Mode-funksjon
void MCP2515_SetMode(uint8_t mode) {
	MCP2515_BitModify(MCP2515_CANCTRL, MCP2515_MODE_MASK, mode);  // Sett mode-bits (bits 5-7) i CANCTRL-registeret
}
