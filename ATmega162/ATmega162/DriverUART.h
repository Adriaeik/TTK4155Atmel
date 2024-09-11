/*
 * DriverUART.h
 *
 * Created: 04.09.2024 15:07:35
 *  Author: adriaeik
 */ 


#ifndef DRIVERUART_H_
#define DRIVERUART_H_

#include "Utils.h"
#include <stdio.h>

#define FOSC 4915200UL  // Sett denne til riktig klokkehastighet
#define BAUD 9600
#define MYUBBR FOSC/16/BAUD-1


void UART_Init(unsigned int ubrr);
void UART_SendChar(char data);
char UART_ReceiveChar(void);
int UART_putChar(char c, FILE *stream);
int UART_getChar(FILE *stream);
void URAT_initStudio();
void UART_EnableReceiveInterrupt(void);
void UART_DisableReceiveInterrupt(void);

#endif /* DRIVERUART_H_ */