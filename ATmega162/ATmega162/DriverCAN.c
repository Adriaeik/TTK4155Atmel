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
	MCP2515_Write(MCP2515_TXB0SIDH, msg->id >> 3);  // SIDH - Standard ID high - KOK: mcp_write(MCP_TXB0SIDH, message->id / 8); // De åtte høyeste bitene i iden.
	MCP2515_Write(MCP2515_TXB0SIDL, (msg->id << 5) & 0xE0);  // SIDL - Standard ID low  - KOK: mcp_write(MCP_TXB0SIDL, (message->id % 8) << 5); // De tre laveste bitene i iden.
	MCP2515_Write(MCP2515_TXB0DLC, msg->length);  // DLC - Data length code
	
	
	for (uint8_t i = 0; i < msg->length; i++) {
		MCP2515_Write(MCP2515_TXB0D0 + i, msg->data[i]);  // Send data byte-by-byte
	}

	// Be MCP2515 om � sende meldingen
	MCP2515_RequestToSend(0x01);  // RTS TX buffer 0
}

void CAN_ReceiveMessage(CANMessage* msg) {
	while (!(MCP2515_ReadStatus() & 0x01));  // Vent til det finst ei melding i RX buffer
	//Tror while loopen er unødvendig?
	
	// Les ID og lengde
	msg->id = (MCP2515_Read(0x61) << 3) + (MCP2515_Read(0x62) >> 5);  // RX buffer 0	
	msg->length = MCP2515_Read(0x65);  // RX buffer 0 DLC

	// Les data
	for (uint8_t i = 0; i < msg->length; i++) {
		msg->data[i] = MCP2515_Read(0x66 + i);  // RX buffer 0 data
	}
	
	/*
	Kok sjekket hvilken buffer som har fått melding
	Vi leser bare av buffer 0
	
	    if (mcp2515_read(MCP_CANINTF(0x2c)) & MCP_RX0IF(0x01)) {
		    // read from buffers, shift to combine high and low bits
		    uint8_t id_low_bits = mcp2515_read(MCP_RXB0SIDL(0x62));
		    uint8_t id_high_bits = mcp2515_read(MCP_RXB0SIDH);
		    message->id = (id_low_bits >> 5) + (id_high_bits << 3);

		    message->data_length = mcp2515_read(MCP_RXB0DLC);

		    for (int m = 0; m < message->data_length; m++) {
			    message->data[m] = mcp2515_read(MCP_RXB0D0 + m);
		    }
	    }

	    else if (mcp2515_read(MCP_CANINTF(0x2c)) & MCP_RX1IF(0x02)) {
		    // read from buffers, shift to combine high and low bits
		    uint8_t id_low_bits = mcp2515_read(MCP_RXB1SIDL);
		    uint8_t id_high_bits = mcp2515_read(MCP_RXB1SIDH);
		    message->id = (id_low_bits >> 5) + (id_high_bits << 3);

		    message->data_length = mcp2515_read(MCP_RXB1DLC);

		    for (int m = 0; m < message->data_length; m++) {
			    message->data[m] = mcp2515_read(MCP_RXB1D0 + m);
		    }
	    }


	Den er fra kok, og gjør akkuratt samme som oss,
	bare at den først sjekker om det er en melding i buffer 0
	-> gjør det vi gjorde
	else if melding i buffer 1
	-> samme som vi gjorde men med adressene til buffer 1
	else: returner 1 (indikasjon på at det ikke er noen melding, sikkert 
	brukt senere i koden til error-handling osv
	*/
}
