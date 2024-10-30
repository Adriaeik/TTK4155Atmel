/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "SRAM.h"
#include "Menu_init.h"
#include "DriverCAN.h"
#include "game.h"

extern uint8_t screen_count = 0;
extern uint8_t menu_pos_count = 0;
extern uint8_t playGame = 0;
extern uint16_t score_counter = 0;
extern uint16_t fake_proc_time = 0; 
extern uint8_t count_fake_proc = 0;

Game main_game;
MultiBoard board;




int main(void) {
	/*_________________INITIALISERINGER START______________________*/
	/*--- Initialiser UART ---*/
	UART_Init(MYUBBR);
	URAT_initStudio();
	UART_EnableReceiveInterrupt();
    setup_timer();						// Start millisekundteljinga
	CAN_Init(MCP2515_MODE_NORMAL);
	
	printf("GO!");
	/*--- Ditta må (noko av det ihvertfall) vere etter sei() ---*/
	externalMemoryInit();				// Initialiser eksternt minne må vere etter sei
	initialize_menus();
	MultiBoard_Init(&board);			// Initialiser MultiBoard og kalibrer joystickens origo
	game_Init(&main_game);
	main_game.lives = 5;
	oled_init();						// Initialiser OLED-skjermen
		/*--- Optional setup ---*/
			//setup_printf_for_oled();		
			//SRAM_test();
	/*_______OLED + LOGO_______*/
	oled_clear_screen();
	/*- Den komboen her var heilt nydelig -*/
	oled_draw_line(10, 10, 100, 50);
	oled_draw_circle(64, 32, 20);
	oled_draw_square(20, 20, 40, 30);
	oled_data_from_SRAM();
	_delay_ms(500);
			
	/*_________________SPI_________________*/
	// Initialiser SPI og MCP2515 i loopback-modus
	
	
    //MCP2515_SetMode(MCP2515_MODE_CONFIG);  // Sett MCP2515 i Configuration Mode
	// Les CANSTAT-registeret (0x0E) for å sjekke om MCP2515 er i loopback-modus
	CAN_Check_startup();
	sei();								// Aktiver globale avbrot
	

	
	
	
	/*______MENY______*/
	extern Menu* current_men;//u = &mainMenu; //kan kanskje teste med å starte i ein anna meny
	write_menu_oled_to_SRAM(current_menu);
	
	print_game_status();
	
	
	//clear_highscore_list();
	//update_highscore_list(10);
	//update_highscore_list(255);
	//update_highscore_list(69);
	//update_highscore_list(1);
	
	
	/*_______HOVUDLØKKE______*/
	 while (1) {

        menu_navigate(&board, current_menu);  // Kallar `menu_navigate` med referanse til gjeldande meny
		
		
		if(screen_count >= 2){
			oled_data_from_SRAM();
			screen_count = 0;
		}
		/*Så lenge vi ikkje har noko delay gåandes og ditta står her tenker eg 
		at den oppdateres automatisk med det minnet vi har skreve til sramen?
		Det kunne vert fornuftig med eit flag her då
		
		16ms skal gi ich 60hz, gitt at resten av programmet kjører raskt nok...
		*/	
			//while(main_game.start_game == 1){
				//oled_write_screen_to_SRAM(&solkors);
				////TIMSK &= ~(1 << TOIE1);
				//
				//MultiBoard_Update(&board);
				//MultiBoard_Send(&board);
				//if (main_game.lives == 0)
				//{
					//currentMenuState = MAIN_MENU;
					//current_menu = &mainMenu;
					//main_game.start_game = 0;
				//}
			//}
			//printf("%d,   %d\r\n", main_game.start_game, main_game.lives);
	 }
	return 0;
}

/*
switch (menu->current_position) {
	case 0:
	oled_write_line_to_SRAM(0, "Startar spelet...");
	playGame = 1;
	main_game.start_game = 1;
*/
ISR(INT0_vect) {
	CANMessage msg;
	CAN_ReceiveMessage(&msg);
		//printf("data[0]: %c adresse = %d\n\r",msg.data[0], msg.id);
		//playGame = 0;
		printf("melding tatt imot ID: %d\n\r", msg.id);
		game_Recive(&main_game, &msg);
		//TIMSK |= (1 << TOIE1);
	/* VI TROR at dette handterast i recive, trøbbel å ha det med*/
		//MCP2515_BitModify(MCP2515_CANINTF, MCP2515_RX1IF | MCP2515_RX0IF, 0xFF);
}

//ISR(TIMER1_COMPA_vect){
	//oled_data_from_SRAM();
//};

// Kjører i 75Hz ish
ISR(TIMER1_OVF_vect) {
	static second_conv = 0;
	screen_count++;
	menu_pos_count++;
	
	if (count_fake_proc){
		fake_proc_time++;
	}    
	
	if(main_game.start_game){
		second_conv++;
		if(second_conv%75 == 0){
			//Har gått ett sekund (eller bittelitt mer)
			score_counter++;
			printf("score++: %d \r\n", score_counter);
		}
	}
}