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
	WDT->WDT_MR = WDT_MR_WDDIS;
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate på 9600
	
	// Kall can_init med CanInit-strukturen og rxInterrupt = 1 for å aktivere mottaksinterrupt
	// Sett opp CAN, definer 1 TX-mailboks og 2 RX-mailboksar
	can_init_def_tx_rx_mb();
	
	

	
	// Konfigurer meldinga som skal sendast
	CAN_MESSAGE msg = {
	69,
	8,
	"TrurDetF"};  // Data til meldinga
	CAN_MESSAGE msg_rec;
	
	uint8_t i = 0;

	while (1) {
		i++;
		msg.id = i;
		// Sjekk om du kan sende meldinga (TX-mailboksen er klar)
		if (!can_send(&msg, 0)) {
			printf("Send melding nr %d no!\n\n\r", i);
		} 
		//if (!can_receive(&msg, 1)) {
			//printf("fikk melding, ID:  %d no!\n\n\r", i);
			//
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
		
	}
}