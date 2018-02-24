#include "hal.h"
#include "inputs.h"
#include "actuators.h" 
#include "ticker.h"

///////////////////////// TEST /////////////////////////

void test (int test_number) {

	unsigned long int i = 0, j = 0;
	const unsigned long int delay = 400000;
	
	switch(test_number) {
		//Test LEDs:
		case 0:
			// PORTB = not(PORTB) and 0b 1  111 1111
			PORTB = ~PORTB & 0xFF;
			for (i=0; i < delay; ++i) {
			}
			break;
		
		//Test Buttons w/LEDs:
		case 1:
			if(PTIH & 0x02){
			// PORTB = not(PORTB) and 0b 1  111 1111
			PORTB = ~PORTB & 0xFF;
			}
			for (i=0; i < delay; ++i) {
			}
			break;

		//Test Beeper:
		case 2:
			if(PTIH & 0x02) {
				beeperOff();  
			}
			else if (!(PTIH& 0x02)) {
				beeperOn(90000);
			}
			break;
		
		//Test Servo:
		case 3:
			while(i < 90) {
				++i;
				for (j = 0; j < 10000; ++j) {
				}
				positionServo (i);
			}
			break;
		   
		default:
			PORTB = 0x55;
			break;
	}

}