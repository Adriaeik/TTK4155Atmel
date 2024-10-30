/*
 * GameSettings.c
 *
 * Created: 27/10/2024 12:47:44
 *  Author: Ravn
 */ 

#include "GameSettings.h"


void settings_init(Game* s){
	s->lives = 5;
	s->sensitivity = 5;
	s->difficulty = 0;
}

void add_lives(Game* s){
	if(s->lives == 9){s->lives = 1; return;}
	
	s->lives++;	
}

void add_sensitivity(Game* s){
	if(s->sensitivity == 9){s->sensitivity = 1; return;}
		
	s->sensitivity++;
}

void add_difficulty(Game* s){
	if(s->difficulty == 2){s->difficulty = 0; return;}
	
	s->difficulty++;
}

void reset_settings(Game* s){
	settings_init(s);
}


