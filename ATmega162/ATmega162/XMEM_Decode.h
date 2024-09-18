/*
 * XMEM_Decode.h
 *
 * Created: 11.09.2024 16:27:39
 *  Author: ravneb
 */ 


#ifndef XMEM_DECODE_H_
#define XMEM_DECODE_H_

#include "Utils.h"

// Definer adresseomr�det og storleik for Ekstern adresse
#define SRAM_START 0x1800
#define SRAM_SIZE  0x0800  // 2KB SRAM
#define ADC_START 0x1400
#define ADC_SIZE 0x0400
#define OLED_START 0x1000
#define OLED_SIZE 0x0400

//Start indexer
#define OLED_START_IDX 0x0000
#define ADC_START_IDX 0x0400
#define SRAM_START_IDX 0x0800

void externalMemoryInit(void);
void SRAM_write(uint16_t addr, uint8_t data);
uint8_t SRAM_read(uint16_t addr);
void Universal_write(volatile uint16_t addr, uint8_t data);
uint8_t Universal_read(volatile uint16_t addr);

uint8_t ADC_read(int channel);

#endif /* XMEM_DECODE_H_ */