#include <mc9s12dp256.h>
#include "inputs.h"

/*
 * Initializes all sensors and inputs
 */

void enableSensors (void) {
  
	// Set PORTH and PORTA as inputs
	DDRH = 0x00;
	DDRA = 0x00;
	
	// Enable interrupts for PORTH
	PIEH = 0xFF;
	//Make it Edge-Triggered   
  PPSH = 0x00;

}