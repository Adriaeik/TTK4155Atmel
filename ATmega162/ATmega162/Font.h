/*
 * Font.h
 *
 * Created: 18.09.2024 00:39:07
 *  Author: eikel
 */ 



#ifndef FONT_H_
#define FONT_H_
#include <avr/pgmspace.h>

extern const uint8_t font8x8_basic[] PROGMEM;
extern const char solkors[128] PROGMEM;
extern const char smiley[128] PROGMEM;
extern const uint8_t emoji_sunglasses_1024[1024] PROGMEM;
extern const char mainMenuItems[8*16] PROGMEM;
extern const char scrollMenuItems[20*16] PROGMEM;
extern const char settingsMenuItems[5*16] PROGMEM;

extern const char null_lives[128] PROGMEM;
extern const char one_lives[128] PROGMEM;
extern const char two_lives[128] PROGMEM;
extern const char three_lives[128] PROGMEM;
extern const char four_lives[128] PROGMEM;
extern const char five_lives[128] PROGMEM;
extern const char six_lives[128] PROGMEM;
extern const char seven_lives[128] PROGMEM;
extern const char eight_lives[128] PROGMEM;
extern const char nine_lives[128] PROGMEM;
extern const char lives_background[128] PROGMEM;

#endif /* FONT_H_ */