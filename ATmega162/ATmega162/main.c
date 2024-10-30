/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "init.h"

extern uint8_t screen_count = 0;
extern uint8_t menu_pos_count = 0;
extern uint8_t playGame = 0;
extern uint16_t score_counter = 0;
extern uint16_t fake_proc_time = 0; 
extern uint8_t count_fake_proc = 0;

Game main_game;
MultiBoard board;
extern Menu* current_menu;



int main(void) {
	/*_________________INITIALISERINGER START______________________*/
	/*--- Initialiser UART ---*/
/*
	UART_Init(MYUBBR);
	URAT_initStudio();
	UART_EnableReceiveInterrupt();
    setup_timer();						// Start millisekundteljinga
	CAN_Init(MCP2515_MODE_NORMAL);
	
	printf("GO!");
	/*--- Ditta må (noko av det ihvertfall) vere etter sei() ---*/
	/*
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
	
	//oled_clear_screen();
	/*- Den komboen her var heilt nydelig -*/
	/*
	oled_draw_line(10, 10, 100, 50);
	oled_draw_circle(64, 32, 20);
	oled_draw_square(20, 20, 40, 30);
	oled_data_from_SRAM();
	_delay_ms(500);
	
	/*_________________SPI_________________*/
	// Initialiser SPI og MCP2515 i loopback-modus
	
	
    //MCP2515_SetMode(MCP2515_MODE_CONFIG);  // Sett MCP2515 i Configuration Mode
	// Les CANSTAT-registeret (0x0E) for å sjekke om MCP2515 er i loopback-modus

	
	//CAN_Check_startup();
	//sei();	// Aktiver globale avbrot


//kaller main init funksjonen
	system_init(&main_game, &board);
	
	/*______MENY______*/
	current_menu = &mainMenu; //kan kanskje teste med å starte i ein anna meny
	write_menu_oled_to_SRAM(current_menu);
	
	print_game_status();
	sei();
	
	
	/*_______HOVUDL�KKE______*/
	while (1) {

        	menu_navigate(&board, current_menu);  // Kallar `menu_navigate` med referanse til gjeldande meny
		
		if(screen_count >= 2){
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
	TIFR |= (1 << TOV1); //Resett TIMER1_OVF_vect
}
