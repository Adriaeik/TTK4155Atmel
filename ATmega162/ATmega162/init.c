#include "init.h"


void system_init(Game* game, MultiBoard* board){
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
	MultiBoard_Init(board);			// Initialiser MultiBoard og kalibrer joystickens origo
	game_Init(game);
	game->lives = 1;
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

	//check if can is in correct mode
	CAN_Check_startup();
	//activate interrupts
	sei();	
}
