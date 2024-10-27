/*
 * init.c
 *
 * Created: 24.10.2024 21:07:49
 *  Author: eikel
 */ 
#include "../include/init.h"

MultiBoard board;
Game main_game;
uint16_t score;

long long int Kp;
long long int Ki;
long long int Kd;
long long int error;
long long int prev_error;
long long int integral;
long long int derivat;

void init(void){
	SystemInit();  // Initialiser systemklokka og mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS; // Trur ditta va watchdogtimer greier
	uart_init(URAT_CPUFREQ, URAT_BAUD);  // Initialiser UART med ein baudrate på 9600
	MultiBoard_Init(&board);
	game_Init(&main_game);
	can_init_def_tx_rx_mb();
	servo_init();
	motor_init();
	IR_Init();
	encoder_init();
	
	checkExternalValues_toScreen();
}

void checkExternalValues_toScreen(void) {
	// bruk \n for enter og \r for å komme tilbake på linja. Lag pen tabellutskrift
	printf("\n===============================================================\n\r");
	printf("| %-15s | %-10s | %-15s |\n\r", "Variabel", "Verdi", "Enhet");
	printf("===============================================================\n\r");
	printf("| %-15s | %-10d | %-15s |\n\r", "LSpos", board.LSpos, "(0-255)");
	printf("| %-15s | %-10d | %-15s |\n\r", "RSpos", board.RSpos, "(0-255)");
	printf("| %-15s | %-10d | %-15s |\n\r", "LBtn", board.LBtn, "(0/1)");
	printf("| %-15s | %-10d | %-15s |\n\r", "RBtn", board.RBtn, "(0/1)");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyBtn", board.JoyBtn, "(0/1)");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyXOrigo", board.JoyXOrigo, "(0-255)");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyYOrigo", board.JoyYOrigo, "(0-255)");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyXposCal", board.JoyXposCal, "(-161 to 94)");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyYposCal", board.JoyYposCal, "(-161 to 94)");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyAngle", board.JoyAngle, "Degrees");
	printf("| %-15s | %-10d | %-15s |\n\r", "JoyYLastAction", board.JoyYLastAction, "Timestamp");
	printf("| %-15s | %-10d | %-15s |\n\r", "Lives", main_game.lives, "Count");
	printf("| %-15s | %-10d | %-15s |\n\r", "Score", main_game.score, "Points");
	printf("| %-15s | %-10d | %-15s |\n\r", "start", main_game.start_game, "Start");
	printf("| %-15s | %-10d | %-15s |\n\r", "Kp", (int)Kp, "Unitless");
	printf("| %-15s | %-10d | %-15s |\n\r", "Ki", (int)Ki, "Unitless");
	printf("| %-15s | %-10d | %-15s |\n\r", "Kd", (int)Kd, "Unitless");
	printf("| %-15s | %-10d | %-15s |\n\r", "Error", (int)error, "Unitless");
	printf("| %-15s | %-10d | %-15s |\n\r", "Prev Error", (int)prev_error, "Unitless");
	printf("| %-15s | %-10d | %-15s |\n\r", "Integral", (int)integral, "Unitless");
	printf("| %-15s | %-10d | %-15s |\n\r", "Derivat", (int)derivat, "Unitless");
	printf("===============================================================\n\n\r");
}