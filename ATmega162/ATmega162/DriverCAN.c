/*
 * DriverCAN.c
 *
 * Created: 22.09.2024 14:04:28
 *  Author: eikel
 */ 

#include "DriverCAN.h"

void CAN_Init(void) {
	// Tilbakestill MCP2515 og sett i loopback-modus
	MCP2515_Reset();

	//// Sett MCP2515 i konfigurasjonsmodus
	//MCP2515_Write(0x0F, 0x80);  // CANCTRL register - Set config mode
	//MCP2515_BitModify(0x2F, 0xE0, 0x40);  // Loopback mode
	
	    // Sett MCP2515 i konfigurasjonsmodus
	    MCP2515_SetMode(0x04);  // Konfigurasjonsmodus

	    // Konfigurer bittiming for 500 kbps (eksempel)
	    MCP2515_Write(0x2A, 0x03);  // CNF1: Baudrate-prescaler
	    MCP2515_Write(0x29, 0xF0);  // CNF2: Propagation and Phase 1 segment
	    MCP2515_Write(0x28, 0x86);  // CNF3: Phase 2 segment

	    // Sett MCP2515 i loopback-modus (0x02 for loopback-modus)
	    MCP2515_SetMode(0x02);  // Loopback-modus
}

void CAN_SendMessage(CANMessage* msg) {
	while (MCP2515_ReadStatus() & 0x04);  // Vent til TX buffer er ledig
	// Send CAN-melding til TX-buffer
	MCP2515_Write(0x31, msg->id >> 3);  // SIDH - Standard ID high
	MCP2515_Write(0x32, (msg->id << 5) & 0xE0);  // SIDL - Standard ID low
	MCP2515_Write(0x35, msg->length);  // DLC - Data length code
	for (uint8_t i = 0; i < msg->length; i++) {
		MCP2515_Write(0x36 + i, msg->data[i]);  // Send data byte-by-byte
	}

	// Be MCP2515 om å sende meldingen
	MCP2515_RequestToSend(0x01);  // RTS TX buffer 0
}

CANMessage CAN_ReceiveMessage(void) {
	CANMessage msg;
	while (!(MCP2515_ReadStatus() & 0x01));  // Vent til det finst ei melding i RX buffer
	// Les ID og lengde
	msg.id = (MCP2515_Read(0x61) << 3) | (MCP2515_Read(0x62) >> 5);  // RX buffer 0
	msg.length = MCP2515_Read(0x65);  // RX buffer 0 DLC

	// Les data
	for (uint8_t i = 0; i < msg.length; i++) {
		msg.data[i] = MCP2515_Read(0x66 + i);  // RX buffer 0 data
	}

	return msg;
}
