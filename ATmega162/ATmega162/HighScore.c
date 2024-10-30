/*
 * HighScore.c
 *
 * Created: 27/10/2024 15:13:40
 *  Author: Ravn
 */ 

#include "HighScore.h"


void clear_highscore_list(void){
	for(uint8_t i = 0; i < 20; i++){
		SRAM_write(1024+i*16, 0);
	}
}

void printf_highscore_list(void){
	for(uint8_t i = 0; i < 20; i++){
		uint16_t score = SRAM_read(1024+i*16);
		printf("Number %d: %d \r\n", i, score);
	}
}


void update_highscore_list(uint16_t new_score) {
	uint16_t current_score;
	int insert_index = -1;

	// Finn riktig plassering for ny score ved å iterere over highscore-listen
	for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
		current_score = (uint16_t)SRAM_read(HS_START_IDX + i * 16);

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
		current_score = (uint16_t)SRAM_read(HS_START_IDX + (i - 1) * 16);
		SRAM_write(HS_START_IDX + i * 16, current_score);
	}

	// Sett inn den nye poengsummen på riktig plass
	SRAM_write(HS_START_IDX + insert_index * 16, new_score);
}


void highscore_to_SRAM(uint8_t scroll_offset){
	uint8_t k = 0;
	if(scroll_offset == 0){
		k = 1;
	}
	for(k; k < 8; k++){
		uint16_t score = SRAM_read(HS_START_IDX + scroll_offset*16 + (k-1)*16);
		uint8_t score_len = count_digits(score);
		char score_as_string[score_len+1];
		number_to_chars(score, score_as_string, score_len);
		
		for(uint8_t i = 0; i < score_len; i++){
			oled_write_char_to_SRAM(k, 15-i, score_as_string[score_len-i-1]);
		}
		
	}
}
	







