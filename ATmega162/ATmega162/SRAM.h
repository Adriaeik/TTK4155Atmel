/*
 * SRAM.h
 *
 * Created: 04.09.2024 18:44:34
 *  Author: adriaeik
 */ 
#include <stdlib.h>
#include "Utils.h"

#ifndef SRAM_H_
#define SRAM_H_
// Definer adresseområdet for SRAM
#define SRAM_START 0x1800
#define SRAM_SIZE  0x0800  // 2KB SRAM

void externalMemoryInit(void);
void SRAM_write(uint16_t addr, uint8_t data);
uint8_t SRAM_read(uint16_t addr);
void SRAM_test(void);
#endif /* SRAM_H_ */