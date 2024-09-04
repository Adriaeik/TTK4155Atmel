/*
 * DriverUART.h
 *
 * Created: 04.09.2024 15:07:35
 *  Author: adriaeik
 */ 


#ifndef DRIVERUART_H_
#define DRIVERUART_H_

#include "Utils.h"

#define FOSC 1843200UL  // Sett denne til riktig klokkehastighet
#define BAUD 9600
#define MYUBBR FOSC/16/BAUD-1


void UART_Init(unsigned int ubrr);
void UART_SendChar(char data);
char UART_ReceiveChar(void);
void UART_EnableReceiveInterrupt(void);
void UART_DisableReceiveInterrupt(void);

#endif /* DRIVERUART_H_ */