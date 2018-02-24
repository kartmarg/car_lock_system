/*
	Author: Martin Karg
*/

#include "hal.h"
#include "inputs.h"
#include "actuators.h" 
#include "ticker.h"
#include "canprocessing.h"
#include "stateprocessing.h"

#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"

// Declaration of Constants

// The ID of each door is set following the design document
// as a numeric constant

#define DOOR_ID 1 // Front Left
//#define DOOR_ID 2 // Front Right
//#define DOOR_ID 3 // Rear Right
//#define DOOR_ID 4 // Rear Left
//#define DOOR_ID 5 // Trunk

// A numeric value is assigned to each input possibility

#define CRASH_ON_IN 1 
#define CRASH_OFF_IN 2
#define SPEED_ON_IN 3
#define SPEED_OFF_IN 4
#define LIGHTS_ON_IN 5
#define LIGHTS_OFF_IN 6
#define CHILD_LOCK_ON_IN 7
#define CHILD_LOCK_OFF_IN 8
#define CENTRAL_LOCK_IN 9
#define CENTRAL_UNLOCK_IN 10
#define DOOR_EXT_OPEN_IN 11
#define DOOR_INT_OPEN_IN 12
#define DOOR_LOCK_UNLOCK_IN 13
#define LOCK_ALL_IN 14
#define UNLOCK_ALL_IN 15
#define FRONT_RIGHT_UNLOCKED_IN 16
#define FRONT_RIGHT_LOCKED_IN 17
#define REAR_LEFT_UNLOCKED_IN 18
#define REAR_LEFT_LOCKED_IN 19
#define REAR_RIGHT_UNLOCKED_IN 20
#define REAR_RIGHT_LOCKED_IN 21
#define TRUNK_UNLOCKED_IN 22
#define TRUNK_LOCKED_IN 23
#define THEFT_AVOIDANCE_ON_IN 24
#define THEFT_AVOIDANCE_OFF_IN 25

// Declaration of global variables
int Last_Input = 0;
int New_Input = 0;

int MS_10 = 0;
int MS_250 = 0;

void main(void) {

  unsigned long int i = 0;
	const unsigned long int delay = 500;

	EnableInterrupts;  // This is necessary for the debugger

	enableSensors();
	enableActuators();
	
	positionServo(-90);

	for(;;) {
		if(MS_10 == 1){
			MS_250 = MS_250 + 1;
			if(MS_250 >= 25){
				writeKeepAlive();
			}
		}
		
		processInput(checkInput());
		processState();		
	}
 
}

// Check inputs
int checkInputs(){
	int input = -1;
	
	New_Input = PTIA;
	
	if (New_Input != Last_Input){
		if ((New_Input & 1) == 1)
			input = DOOR_INT_OPEN_IN;
		if (((New_Input >> 1) & 1) == 1)
			input = DOOR_EXT_OPEN_IN;
		if (((New_Input >> 2) & 1) == 1)
			input = DOOR_EXT_OPEN_IN;
		if (((New_Input >> 3) & 1) == 1)
			input = LIGHTS_ON_IN;
		else if (((New_Input >> 3) & 1) == 0)
			input = LIGHTS_OFF__IN;
		if (((New_Input >> 4) & 1) == 1)
			input = SPEED_ON_IN;
		else if (((New_Input >> 4) & 1) == 0)
			input = SPEED_OFF__IN;
		if (((New_Input >> 5) & 1) == 1)
			input = CHILD_LOCK_ON_IN;
		else if (((New_Input >> 5) & 1) == 0)
			input = CHILD_LOCK_OFF_IN;
		if (((New_Input >> 6) & 1) == 1)
			input = DOOR_LOCK_UNLOCK_ON_IN;
		else if (((New_Input >> 6) & 1) == 0)
			input = DOOR_LOCK_UNLOCK_OFF_IN;
		if (((New_Input >> 7) & 1) == 1)
			input = DOOR_CLOSED_IN;
	}
	
	Last_Input = New_Input;
	input = Last_Input;
	
	return input;
}

//PTH Interrupt
interrupt (((0x10000-Vporth)/2)-1) void PORTH_ISR(void) {
	// De-bouncing solution
	MSDelay (1);
	//Test for any interrupt
	//interruptTest();
	if(!(PTIH & 0x01)){
	  
	}
	if(!(PTIH & 0x02)){
	  
	}
	if(!(PTIH & 0x04)){
	  
	}
	if(!(PTIH & 0x08)){
		
	  
	}
	if(!(PTIH & 0x10)){
	  
	}
	if(!(PTIH & 0x20)){
	  
	}
	PIFH = PIFH | 0xFF;      //clear PTH Interupt Flags for the next round. Writing HIGH will clear the Interrupt flags
} 

// Function for required delays
void MSDelay(unsigned int itime) {
  
  unsigned int i; 
  unsigned int j;
  
  for(i=0;i<itime;i++) {
    for(j=0;j<4000;j++);
  }
}