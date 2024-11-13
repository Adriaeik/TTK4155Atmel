/*
 * HighScore.c
 *
 * Created: 27/10/2024 15:13:40
 *  Author: Ravn
 */ 

#include "HighScore.h"


void clear_highscore_list(void){
	for(uint8_t i = 0; i < 20; i++){
		SRAM_write(1024+i*2, 0b00000000);
		SRAM_write(1024+i*2+1, 0b00000000);
	}
}



uint16_t get_highscore_value(uint8_t index){
	uint16_t value = 0;
	
	value = (uint16_t)SRAM_read(HS_START_IDX+2*index) + (uint16_t)256*SRAM_read(HS_START_IDX+2*index+1);
	return value;
}

void place_highscore_value(uint8_t index, uint16_t value){
	uint8_t valuehigh = value/255;
	uint8_t valuelow = (uint8_t)value%255;
	
	//printf("%d + %d =? %d \r\n", valuehigh, valuelow, value);
	
	SRAM_write(HS_START_IDX+2*index, valuelow);
	SRAM_write(HS_START_IDX+2*index+1, valuehigh);
	
	//
	//uint8_t vallow = SRAM_read(HS_START_IDX+2*index);
	//uint16_t valhi = SRAM_read(HS_START_IDX+2*index+1) * 256;
	//uint16_t readval = valhi+vallow;
	//
	////printf("%d og %d =? %d og %d \r\n", valuehigh, valuelow, valhi, vallow);
	//printf("Skrev: %d, leste: %d \r\n", value, readval);
}


void update_highscore_list(uint16_t new_score) {
	uint16_t current_score;
	int insert_index = -1;

	// Finn riktig plassering for ny score ved å iterere over highscore-listen
	for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
		current_score = get_highscore_value(i);

		// Når vi finner en poengsum som er mindre enn ny poengsum, marker vi innsettingspunktet
		if (new_score > current_score) {
			insert_index = i;
			break;
		}
	}

	// Hvis ny poengsum ikke er høy nok til å kvalifisere, avslutt
	if (insert_index == -1) {
		return;
	}

	// Flytt elementene nedover for å lage plass til ny score
	for (int i = HIGHSCORE_LIST_SIZE - 1; i > insert_index; i--) {
		current_score = get_highscore_value(i-1);
		place_highscore_value(i, current_score);
	}

	// Sett inn den nye poengsummen på riktig plass
	place_highscore_value(insert_index, new_score);
}


void highscore_to_SRAM(uint8_t scroll_offset){
	uint8_t k = 0;
	if(scroll_offset == 0){
		k = 1;
	}
	for(;k < 8; k++){
		uint16_t score = get_highscore_value(scroll_offset + (k-1));
		uint8_t score_len = count_digits(score);
		char score_as_string[score_len+1];
		number_to_chars(score, score_as_string, score_len);
		
		for(uint8_t i = 0; i < score_len; i++){
			oled_write_char_to_SRAM(k, 15-i, score_as_string[score_len-i-1]);
		}
		
	}
}
	







