 /* $Rev:$
 *
 */

#include <mc9s12dp256.h>
#include "pwm.h"
#include "beeper.h"

//****************Initialize LEDs***************//
void enableLEDs() {
	// Deactivate the 7 segment display
	DDRP = DDRP | 0xF; 	// Data Direction Register Port P
	PTP  = PTP | 0x0F; 	// Unlock all segments of ptp

	// Activate the LEDs
	DDRJ_DDRJ1 	= 1;	// Data Direction Register Port J
	PTJ_PTJ1 	= 0;  	// Unlock all LEDs

	// Set PORTB as output (PORTB is for LEDs)
	DDRB        = 0xFF; // Data Direction Register Port B
	PORTB = 0x00; 
}

//*******************Set LEDs******************//
// Set LEDs in PORTB to the intial parameter 
// unsigned char set_leds
void setLeds(unsigned char set_leds){
	PORTB = set_leds;
}

//*******************Get LEDs******************//
// Get LEDs values in PORTB to retun parameter 
// unsigned char current_leds
unsigned char getLeds(){
	return PORTB;
}

//*****************Turn LED On****************//
// Turns LED x on, where x is the initial parameter
// unsigned char led_number 
void ledOn(unsigned char led_number){
	PORTB |= 1 << led_number;
}

//*****************Turn LED Off****************//
// Turns LED x off, where x is the initial parameter
// unsigned char led_number 
void ledOff(unsigned char led_number){
	PORTB &= ~(1 << led_number);
}

//*****************Toggle LED****************//
// Toggles LED x, where x is the initial parameter
// unsigned char led_number 
void ledToggle(unsigned char led_number){
	PORTB ^= 1 << led_number;
}

//****************Initialize Acts***************//
void enableActuators () {
	/*
	LACT    = PP7 | Servomotor

	FL      = PB7 | LED
	FR      = PB6 | LED
	RL      = PB5 | LED
	RR      = PB4 | LED

	Beeper  = PT5 | Beeper
	 
	*/ 

	// Initialize LEDs
	enableLEDs();

	// Initialize the beeper
	initBeeper();

	// Initialize PWM for servomotor
	openPWM();
  
}

// Lock is open at -90 degrees (0 degrees)
// Lock is open at 90 degrees (180 degrees)
void lockPosition (int position) {
	positionServo(position);
}

void beep () {
	
}