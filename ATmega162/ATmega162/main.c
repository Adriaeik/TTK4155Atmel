/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "init.h"

volatile extern uint8_t screen_count = 0;
volatile extern uint8_t menu_pos_count = 0;
extern uint8_t playGame = 0;
volatile extern uint16_t score_counter = 0;
volatile extern uint16_t fake_proc_time = 0; 
volatile extern uint8_t count_fake_proc = 0;
volatile extern uint8_t second_conv = 0;
volatile extern uint8_t startGame_l = 0;

Game main_game;
MultiBoard board;
extern Menu* current_menu;

 

int main(void) {
	/*_________________INITIALISERINGER START______________________*/
	system_init(&main_game, &board);
	
	/*______MENY______*/
	current_menu = &mainMenu; //kan kanskje teste med å starte i ein anna meny
	write_menu_oled_to_SRAM(current_menu);
	
	print_game_status();
	
	
	sei();
	
	uint8_t screen_count_temp = 0;
	/*_______HOVUDL�KKE______*/
	while (1) {

        menu_navigate(&board, current_menu);  // Kallar `menu_navigate` med referanse til gjeldande meny
		
		
		screen_count_temp = screen_count;
		if(screen_count_temp >= 2){
			oled_data_from_SRAM();
			screen_count = 0;
		}
	 }
	return 0;
}

//creating some Interrupt service routines ISRs

ISR(INT0_vect) {
	CANMessage msg;
	CAN_ReceiveMessage(&msg);
	printf("melding tatt imot ID: %d\n\r", msg.id);
	game_Recive(&main_game, &msg);
}

// Kjører i 75Hz ish
ISR(TIMER1_OVF_vect) {
	screen_count++;
	menu_pos_count++;
	
	if (count_fake_proc){
		fake_proc_time++;
	}    
	
	if(startGame_l){
		second_conv++;
		if(second_conv%75 == 0){
			//Har gått ett sekund (eller bittelitt mer)
			score_counter++;
		}
	}
	 TIFR |= (1 << TOV1); //Resett TIMER1_OVF_vect
}
