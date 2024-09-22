/*
 * DriverSPI.h
 *
 * Created: 22.09.2024 13:37:16
 *  Author: eikel
 */ 


#ifndef DRIVERSPI_H_
#define DRIVERSPI_H_
#include "Utils.h"  // Inkluderer dine makroer

// SPI-funksjoner
void SPI_Init(void);          // Initialiserer SPI-grensesnittet
void SPI_Transmit(uint8_t data);  // Sender byte over SPI
uint8_t SPI_Receive(void);     // Mottar byte over SPI


#endif /* DRIVERSPI_H_ */