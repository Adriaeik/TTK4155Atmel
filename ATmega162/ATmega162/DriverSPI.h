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

// SPI-funksjoner
void SPI_MasterInit(void);          // Initialiserer SPI-grensesnittet
void SPI_Transmit(char data);  // Sender byte over SPI
uint8_t SPI_Receive(void);     // Mottar byte over SPI


#endif /* DRIVERSPI_H_ */