/*
 * DriverCAN.c
 *
 * Created: 22.09.2024 14:04:28
 *  Author: eikel
 */ 

#include "DriverCAN.h"

void CAN_Init(uint8_t mode) {
	MCP2515_init();
	MCP2515_SetMode(mode);	
}

//KOKEN har litt anerledes på dissa to funksjonane - vhat sa dei var funksjonelt like
void CAN_SendMessage(CANMessage* msg) {
	//while (MCP2515_ReadStatus() & 0x04);  // Vent til TX buffer er ledig
	// Send CAN-melding til TX-buffer
	MCP2515_Write(MCP2515_TXB0SIDH, msg->id >> 3);  // SIDH - Standard ID high - KOK: mcp_write(MCP_TXB0SIDH, message->id / 8); // De åtte høyeste bitene i iden.
	MCP2515_Write(MCP2515_TXB0SIDL, ((msg->id & 0x07) << 5));// & MCP2515_CANSTAT);  // SIDL - Standard ID low  - KOK: mcp_write(MCP_TXB0SIDL, (message->id % 8) << 5); // De tre laveste bitene i iden.
	MCP2515_Write(MCP2515_TXB0DLC, msg->length);  // DLC - Data length code
	for (uint8_t i = 0; i < msg->length; i++) {
		MCP2515_Write(MCP2515_TXB0D0 + i, msg->data[i]);  // Send data byte-by-byte
	}

	// Be MCP2515 om � sende meldingen
	MCP2515_RequestToSend(0x01);  // RTS TX buffer 0
}

uint8_t CAN_ReceiveMessage(CANMessage* msg) {
	// Sjekk om det finst ei melding i RX bufferet ved å lese status
	//if (!(MCP2515_ReadStatus() & 0x01)) {
		//return 1;  // Ingen melding tilgjengeleg, returner feil
	//}

	// Les ID og lengde
	msg->id = (MCP2515_Read(MCP2515_RXB0SIDH) << 3) + (MCP2515_Read(MCP2515_RXB0SIDL) >> 5);
	msg->length = MCP2515_Read(MCP2515_RXB0DLC);  // RX buffer 0 DLC

	// Sjekk om meldingslengda er gyldig (0-8 bytes for CAN)
	if (msg->length > 8) {
		return 1;  // Ugyldig lengde, returner feil
	}

	// Les data dersom lengda er gyldig
	for (uint8_t i = 0; i < msg->length; i++) {
		msg->data[i] = MCP2515_Read(MCP2515_RXB0D0 + i);  // RX buffer 0 data
	}

	return 0;  // Melding motteken utan feil
}
