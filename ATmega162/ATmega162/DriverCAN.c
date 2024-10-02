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
	while (MCP2515_ReadStatus() & 0x04);  // Vent til TX buffer er ledig
	// Send CAN-melding til TX-buffer
	//MCP2515_Write(MCP2515_TXB0SIDH, msg->id / 8);  // SIDH - Standard ID high - KOK: mcp_write(MCP_TXB0SIDH, message->id / 8); // De åtte høyeste bitene i iden.
	//MCP2515_Write(MCP2515_TXB0SIDL,  (msg->id % 8) << 5);// & MCP2515_CANSTAT);  // SIDL - Standard ID low  - KOK: mcp_write(MCP_TXB0SIDL, (message->id % 8) << 5); // De tre laveste bitene i iden.
	//
	// set ID and length
	uint16_t id = msg->id & 0x07FF;  // Sørg for at ID er 11-bits
	uint8_t id_high = id >> 3;
	uint8_t id_low = (id & 0x07) << 5;
	//printf("ID HIGH: %d,  ID LOW: %d, Satt saman i Send: %d \n\r",id_high, id_low, (id_high << 3) | (id_low >> 5) );
	MCP2515_Write(MCP2515_TXB0SIDH, id_high);
	MCP2515_Write(MCP2515_TXB0SIDL, id_low & 0xE0);
	MCP2515_Write(MCP2515_TXB0DLC, msg->length);  // DLC - Data length code
	for (uint8_t i = 0; i < msg->length; i++) {
		MCP2515_Write(MCP2515_TXB0D0 + i, msg->data[i]);  // Send data byte-by-byte
	}

	// Be MCP2515 om � sende meldingen
	MCP2515_RequestToSend(MCP2515_RX0IF);
}

uint8_t CAN_ReceiveMessage(CANMessage* msg) {
	// Sjekk om det finst ei melding i RX bufferet ved å lese status
	if (!(MCP2515_ReadStatus() & MCP2515_TX0IF)) {
		return 1;  // Ingen melding tilgjengeleg, returner feil
	}
	uint8_t id_high = MCP2515_Read(MCP2515_RXB0SIDH);
	uint8_t id_low = MCP2515_Read(MCP2515_RXB0SIDL);
	if  (id_low  == 0){ // (0 == 1) {//
		//printf("ID HIGH: %d,  ID LOW: %d, Satt saman i Reci: %d \n\r",id_high, id_low,  8+8*id_high);
		msg->id = 8+8*id_high;
	}else{
		//printf("ID HIGH: %d,  ID LOW: %d, Satt saman i Reci: %d \n\r",id_high, id_low, (id_high << 3) | (id_low >> 5) );
		msg->id = (((id_high << 3) & 0x7f8) | (id_low >> 5));
	}

	
	msg->length = MCP2515_Read(MCP2515_RXB0DLC);  // RX buffer 0 DLC

	// Sjekk om meldingslengda er gyldig (0-8 bytes for CAN)
	if (msg->length > 8) {
		return 1;  // Ugyldig lengde, returner feil
	}
	
	// Les data dersom lengda er gyldig
	for (uint8_t i = 0; i < msg->length; i++) {
		msg->data[i] = MCP2515_Read(MCP2515_RXB0D0 + i);  // RX buffer 0 data
	}
	MCP2515_BitModify(MCP2515_CANINTF, MCP2515_RX0IF, 0);
	//MCP2515_BitModify(MCP2515_CANINTF, 1, 0); // set interupt vector 1 to 0
	//MCP2515_BitModify(MCP2515_CANINTF, 2, 0); // set interupt vector 2 to 0
	return 0;  // Melding motteken utan feil
}
