#ifndef INIT_H_
#define INIT_H_
#include "game.h"
#include "MultiBoard.h"
#include "DriverUART.h"
#include "SRAM.h"
#include "Menu_init.h"
#include "DriverCAN.h"



void system_init(Game* game, MultiBoard* board);

#endif
