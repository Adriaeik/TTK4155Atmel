/*
 * Utils.c
 *
 * Created: 04.09.2024 17:16:46
 *  Author: adriaeik
 */ 
#include "Utils.h"

// Funksjon for å berekne omtrentleg vinkel som eit heiltal (0–360 grader)
int calculateJoystickAngle(int16_t x, int16_t y) {
    int angle = 0;

    // Unngå divisjon med 0 (senterpunkt)
    if (x == 0 && y == 0) {
        return 0;  // Midtpunkt, ingen vinkel
    }

    // Beregn ein enkel vinkel basert på kvadrantar
    if (x >= 0 && y >= 0) {
        // Første kvadrant
        angle = (y * 90) / (x + y);  // Enklare tilnærming
    } else if (x < 0 && y >= 0) {
        // Andre kvadrant
        angle = 90 + ((-x * 90) / (-x + y));
    } else if (x < 0 && y < 0) {
        // Tredje kvadrant
        angle = 180 + ((-y * 90) / (-x - y));
    } else if (x >= 0 && y < 0) {
        // Fjerde kvadrant
        angle = 270 + ((x * 90) / (x - y));
    }

    return angle;
}
