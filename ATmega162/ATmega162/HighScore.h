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
#define HIGHSCORE_LIST_SIZE 19 

void clear_highscore_list(void);

uint16_t get_highscore_value(uint8_t index);
void place_highscore_value(uint8_t index, uint16_t value);
void update_highscore_list(uint16_t new_score);

//Må fikses: Siden vi bruker uint16 og hver adresse er 8bit må det gjøres noe multiplexing i lesing (ogg sikkert skriving) av higscores. 
void highscore_to_SRAM(uint8_t scroll_offset);

//Hjelpefunksjon, kan flyttes til utils





#endif /* HIGHSCORE_H_ */