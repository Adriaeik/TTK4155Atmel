/*
 * DriverCAN.c
 *
 * Created: 22.09.2024 14:04:28
 *  Author: eikel
 */ 

#include "DriverCAN.h"

void CAN_Init(void) {
	MCP2515_init();
	//// Tilbakestill MCP2515 og sett i loopback-modus
	//MCP2515_Reset();
	//
	//// Sett MCP2515 i konfigurasjonsmodus
	//MCP2515_Write(0x0F, 0x80);  // CANCTRL register - Set config mode
	//
	//
	//
	//uint8_t value = MCP2515_Read(0x0E);
	//if((value & 0xE0) != 0x80){
		//printf("ERROR: MCP2515 er ikke i config mode etter reset\r\n");
	//}
	//else{printf("MCP2515 er i config mode etter reset\r\n");}
//
	//MCP2515_BitModify(0x0F, 0xE0, 0x40);  // Loopback mode
	
}

//KOKEN har litt anerledes på dissa to funksjonane - vhat sa dei var funksjonelt like
void CAN_SendMessage(CANMessage* msg) {
	while (MCP2515_ReadStatus() & 0x04);  // Vent til TX buffer er ledig
	// Send CAN-melding til TX-buffer
	MCP2515_Write(0x31, msg->id >> 3);  // SIDH - Standard ID high - KOK: mcp_write(MCP_TXB0SIDH, message->id / 8); // De åtte høyeste bitene i iden.
	MCP2515_Write(0x32, (msg->id << 5) & 0xE0);  // SIDL - Standard ID low  - KOK: mcp_write(MCP_TXB0SIDL, (message->id % 8) << 5); // De tre laveste bitene i iden.
	MCP2515_Write(0x35, msg->length);  // DLC - Data length code
	for (uint8_t i = 0; i < msg->length; i++) {
		MCP2515_Write(0x36 + i, msg->data[i]);  // Send data byte-by-byte
	}

	// Be MCP2515 om � sende meldingen
	MCP2515_RequestToSend(0x01);  // RTS TX buffer 0
}

CANMessage CAN_ReceiveMessage(void) {
	CANMessage msg;
	while (!(MCP2515_ReadStatus() & 0x01));  // Vent til det finst ei melding i RX buffer
	// Les ID og lengde
	msg.id = (MCP2515_Read(0x61) << 3) + (MCP2515_Read(0x62) >> 5);  // RX buffer 0
	msg.length = MCP2515_Read(0x65);  // RX buffer 0 DLC

	// Les data
	for (uint8_t i = 0; i < msg.length; i++) {
		msg.data[i] = MCP2515_Read(0x66 + i);  // RX buffer 0 data
	}

	return msg;
}
