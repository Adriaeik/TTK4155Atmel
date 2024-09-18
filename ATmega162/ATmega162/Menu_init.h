/*
 * Menu_init.h
 *
 * Created: 18.09.2024 23:11:45
 *  Author: eikel
 */ 


#ifndef MENU_INIT_H
#define MENU_INIT_H

#include "Menu.h"
#include <avr/pgmspace.h>

// Funksjon for å initialisere alle menyane
void initialize_menus();

// Menyobjekt-eksportar
extern Menu mainMenu;
extern Menu scrollMenu;
extern Menu settingsMenu;

#endif /* MENU_INIT_H */
