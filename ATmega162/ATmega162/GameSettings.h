/*
 * GameSettings.h
 *
 * Created: 27/10/2024 12:47:29
 *  Author: Ravn
 */ 


#ifndef GAMESETTINGS_H_
#define GAMESETTINGS_H_
#include "utils.h"

typedef struct {
	uint8_t lives;
	uint8_t sensitivity;	
	uint8_t difficulty;
} Settings;

extern Settings settings;

void settings_init(Settings* s);
void add_lives(Settings* s);
void add_sensitivity(Settings* s);
void add_difficulty(Settings* s);
void reset_settings(Settings* s);



#endif /* GAMESETTINGS_H_ */