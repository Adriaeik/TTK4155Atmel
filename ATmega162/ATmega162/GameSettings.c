/*
 * GameSettings.c
 *
 * Created: 27/10/2024 12:47:44
 *  Author: Ravn
 */ 

#include "GameSettings.h"

Settings settings = {5, 5};

void settings_init(Settings* s){
	s->lives = 5;
	s->sensitivity = 5;
	s->difficulty = 1;
}

void add_lives(Settings* s){
	if(s->lives == 9){s->lives = 1; return;}
	
	s->lives++;	
}

void add_sensitivity(Settings* s){
	if(s->sensitivity == 9){s->sensitivity = 1; return;}
		
	s->sensitivity++;
}

void add_difficulty(Settings* s){
	if(s->difficulty == 3){s->difficulty = 1; return;}
	
	s->difficulty++;
}

void reset_settings(Settings* s){
	settings_init(s);
}


