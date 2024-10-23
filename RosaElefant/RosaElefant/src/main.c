/*
 * RosaElefant.c
 *
 * Created: 02.10.2024 10:34:01
 * Author : adriaeik
 */ 


#include "sam.h"
#include "../include/uart_int.h"
#include "../include/time.h"
#include "../include/pwm.h"
#include "../lib/can/can_controller.h"
#include "../include/MultiBoard.h"
#include "../include/driver_IR.h"
#include "../include/encoder.h"

extern MultiBoard board;

uint16_t score;

int main(void) {
	SystemInit();  // Initialiser systemklokka og mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS;
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate på 9600
	MultiBoard_Init(&board);
	// Kall can_init med CanInit-strukturen og rxInterrupt = 1 for å aktivere mottaksinterrupt
	// Sett opp CAN, definer 1 TX-mailboks og 2 RX-mailboksar
	can_init_def_tx_rx_mb();
	servo_init();
	IR_Init();
	encoder_init();
	motor_init();
	
	
	
	// Konfigurer meldinga som skal sendast
	CAN_MESSAGE msg = {
	69,
	8,
	"TrurDetF"};  // Data til meldinga
	CAN_MESSAGE msg_rec;
	
	uint8_t i = 0;
	

	/*for(int i = 0; i < 10; i++){
		if (!can_send(&msg, 0)) {
		printf("Send melding nr %d no!\n\n\r", i);
		}
	}*/
	//can_send(&msg,0);

	while (1) {
		//i++;
		//msg.id = i;
		//// Sjekk om du kan sende meldinga (TX-mailboksen er klar)
		////if (!can_send(&msg, 0)) {
			////printf("Send melding nr %d no!\n\n\r", i);
		////} 
		//if (!can_receive(&msg, 0)) {
			//printf("fikk melding, ID:  %d, data nr 1:  %d no!\n\n\r", msg.id, msg.data[0]);
		//}
		// Legg inn ei forsinking (eller anna logikk)
		//time_spinFor(msecs(1000));

		//// Gå opp til maksimal posisjon (2.1 ms)
		//for (double pos = 0.9; pos <= 2.1; pos += 0.01) {
			//servo_set_position(pos);
			//time_spinFor(msecs(20));  // Vent i 20 ms for glatt overgang
		//}
//
		//// Gå ned til minimum posisjon (0.9 ms)
		//for (double pos = 2.1; pos >= 0.9; pos -= 0.01) {
			//servo_set_position(pos);
			//time_spinFor(msecs(20));  // Vent i 20 ms for glatt overgang
		//}
		IR_Handler();
		printf("score: %d. Encoder val: %d.   Xpos: %d \r\n", score, read_encoder_position(), board.JoyXposCal);
		
		//can_send(&msg,0);
			//time_spinFor(msecs(1000));
		update_motor_control();

		/*	can_send(&msg,0);
			time_spinFor(msecs(1000));*/

		servo_set_position_joy();
		CAN0_Handler();
		
	}
}