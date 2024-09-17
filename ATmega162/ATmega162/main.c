/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "XMEM_Decode.h"
#include "SRAM.h"
#include "MultiBoard.h"
#include "OLED.h"

int main(void) {
	
	// Initialiser UART med baudrate 9600
	UART_Init(MYUBBR);
	
	// Initialiser stdio til � bruke UART
	URAT_initStudio();
	// Aktiver mottaksavbrot
	UART_EnableReceiveInterrupt();
	
	// Aktiver globale avbrot
	sei();
	// Initialiser eksternt minne
	externalMemoryInit();
	// Opprett MultiBoard-struktur for � lagre status
	MultiBoard board;
	// Initialiser MultiBoard og kalibrer joystickens origo
	MultiBoard_Init(&board);
	
	
	SRAM_test();

//TEST OLED////////////////////////////////////////////
	oled_init();  // Initialiser OLED-skjermen
		
	oled_goto_pos(0, 0);  // G� til posisjon (side 0, kolonne 0)
	oled_print_char('A'); // Skriv teiknet 'A'
		
	oled_goto_pos(1, 0);  // G� til neste linje (side 1)
	oled_print_char('B'); // Skriv teiknet 'B'
///////////////////////////////////////////////////////////////
	// Hovudl�kka
	while (1) {
		MultiBoard_Update(&board);
		// Skrive ut alle verdier til UART (for debugging eller logging)
		//printf("Left Slider: %u, Right Slider: %u\n\r", board.LSpos, board.RSpos);
		//printf("Joystick X: %d, Y: %d, Button: %u\n\r", board.JoyXpos, board.JoyYpos, board.JoyBtn);
		//printf("Left Button: %u, Right Button: %u\n\n\r", board.LBtn, board.RBtn);
		//printf("Left Button: %u, Right Button: %u, Button: %u\n\n\r", board.LBtn, board.RBtn, board.JoyBtn);

		// Skriv ut vinkelen
		printf("Joystick X: %d, Y: %d\n\r", board.JoyXposCal, board.JoyYposCal);
		printf("Calculated angle: %d degrees\n\r", board.JoyAngle);
	}
	return 0;
}




