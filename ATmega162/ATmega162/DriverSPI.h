/*
 * DriverSPI.h
 *
 * Created: 22.09.2024 13:37:16
 *  Author: eikel
 */ 


#ifndef DRIVERSPI_H_
#define DRIVERSPI_H_
#include "Utils.h"  // Inkluderer dine makroer
#include "DriverUART.h"

//PINS
#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

// SPI-funksjoner
void SPI_MasterInit(void);          // Initialiserer SPI-grensesnittet
void SPI_Transmit(char data);  // Sender byte over SPI
uint8_t SPI_Receive(void);     // Mottar byte over SPI
void SPI_DselectSlave(void);
void SPI_SelctSlave(void);



#endif /* DRIVERSPI_H_ */