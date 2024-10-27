/*
 * HighScore.h
 *
 * Created: 27/10/2024 15:13:28
 *  Author: Ravn
 */ 


#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

#include "Utils.h"
#include "XMEM_Decode.h"

#define HS_START_IDX 1024
#define HIGHSCORE_LIST_SIZE 20 

void clear_highscore_list(void);

void printf_highscore_list(void);
void update_highscore_list(uint16_t new_score);

void highscore_to_SRAM(uint8_t scroll_offset);

//Hjelpefunksjon, kan flyttes til utils
uint8_t count_digits(uint16_t number);
void number_to_chars(uint16_t number, char* buffer, int length);




#endif /* HIGHSCORE_H_ */