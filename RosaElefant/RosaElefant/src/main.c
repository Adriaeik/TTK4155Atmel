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



int main(void) {
	SystemInit();  // Initialiser systemklokka og mikrokontrolleren
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate på 9600
	
	// Kall can_init med CanInit-strukturen og rxInterrupt = 1 for å aktivere mottaksinterrupt
	// Sett opp CAN, definer 1 TX-mailboks og 2 RX-mailboksar
	can_init_def_tx_rx_mb();
	
	// Sett opp baudrate for 250 kbps (BRP = 17 og TQ = 10)
	uint32_t can_baudrate = CAN_BR_PHASE2(4) | CAN_BR_PHASE1(3) | CAN_BR_PROPAG(2) | CAN_BR_SJW(1) | CAN_BR_BRP(17);
	CAN0->CAN_BR = can_baudrate;
	
	// Konfigurer meldinga som skal sendast
	CAN_MESSAGE msg;
	msg.id = 69;  // ID på meldinga
	msg.data_length = 8;  // Antall byte data
	strcpy((char*)msg.data, "TrurDetF");  // Data til meldinga
	
	uint8_t i = 0;

	while (1) {
		// Sjekk om du kan sende meldinga (TX-mailboksen er klar)
		if (!can_send(&msg, 0)) {
			printf("Sendte melding nr %d no!\n\n\r", i);
			i++;
		}
		
		// Les meldinga frå RX-mailboksen
		if (!can_receive(&msg, 1)) {  // Bruk den første mottaksboks
			printf("Mottok melding med ID: %X, Data[0]: %d\n\r", msg.id, msg.data[0]);
		}
		// Legg inn ei forsinking (eller anna logikk)
		time_spinFor(msecs(1000));

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
	}
}