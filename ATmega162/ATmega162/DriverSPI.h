/*
 * DriverSPI.h
 *
 * Created: 22.09.2024 13:37:16
 *  Author: eikel
 */ 


#ifndef DRIVERSPI_H_
#define DRIVERSPI_H_
#include "Utils.h"  // Inkluderer dine makroer

void SPI_MasterInit(void); 
uint8_t SPI_MasterTransmit(uint8_t data);	// Send ein byte gjennom SPI og motta data frå slave
void SPI_SelectSlave(void);					// Velje MCP2515 (sett Slave Select-linja lågt)
void SPI_DeselectSlave(void);				// Frigjere MCP2515 (sett Slave Select-linja høgt)
void MCP2515_SendCommand(uint8_t command);	// MCP2515: Send kommando
uint8_t MCP2515_ReadStatus(void);			// MCP2515: Les status



#endif /* DRIVERSPI_H_ */