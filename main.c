/*
	Author: Martin Karg
*/

#include "hal.h"
#include "inputs.h"
#include "actuators.h" 
#include "lcddrv.h"
#include "ticker.h"
#include "candriver.h"
#include "canprocessing.h"
#include "stateprocessing.h"

#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"

// Declaration of Constants

// A numeric value is assigned to each state

#define THEFT_AVOIDANCE_ST 1
#define LOCKED_ST 2
#define UNLOCKED_ST 3
#define OPEN_ST 4
#define CLOSED_ST 5

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
#define DOOR_LOCK_UNLOCK_ON_IN 24
#define DOOR_LOCK_UNLOCK_OFF_IN 25
#define DOOR_CLOSED_IN 26


// Declaration of global variables
unsigned char last_Input = 0;
unsigned char new_Input = 0;
int  val = 0;

int MS_10 = 0;
int MS_250 = 0;

void MSDelay(unsigned int itime);

void main(void) {

	unsigned long int i = 0;
	const unsigned long int delay = 500;
	
	last_Input = 0;
    new_Input = PORTA;

	EnableInterrupts;  // This is necessary for the debugger

	enableSensors();
	enableActuators();
	
	initLcd();
	initCAN();
	
	positionServo(-90);

	for(;;) {
		if(MS_10 == 1){
			MS_250 = MS_250 + 1;
			if(MS_250 >= 25){
				writeKeepAlive();
			}
		}
		
		if(checkInputs() != -1){// if there's an input
			processInput(val); //val will take the value of the desired state
			processState();
		}
		
		refreshDisplay();//used to check the change in the states
		
	}
 
}

void tick10ms(){
  MS_10 = 1;  
}

// Check inputs
int checkInputs(){
	int input = -1;
	
    unsigned char resul;
    
    new_Input = PORTA;   //TODO port a
    
    if (new_Input != last_Input){
      resul = new_Input ^ last_Input;
      switch(resul){
        case 0x01:
        	if((new_Input & resul) == resul )
            	input=DOOR_INT_OPEN_IN;
        	break;
        case 0x02:
        	if((new_Input & resul) == resul)
        		input =  DOOR_EXT_OPEN_IN ;
        	break;
        case 0x04:
          //input = DOOR_EXT_OPEN_IN
        	break;
        case 0x08:
        	if((new_Input & resul) == resul)
            	input = LIGHTS_ON_IN;
        	else
            	input = LIGHTS_OFF_IN;
        	break;
        case 0x10:
        	if((new_Input & resul) == resul)
            	input = SPEED_ON_IN;
        	else
            	input= SPEED_OFF_IN;
        	break;
        case 0x20:
        	if((new_Input & resul) == resul)
            	input = CHILD_LOCK_ON_IN;
        	else
            	input = CHILD_LOCK_OFF_IN;
        	break;
        case 0x40:
			if((new_Input & resul) == resul)
				input = DOOR_LOCK_UNLOCK_ON_IN  ;
			else
				input = DOOR_LOCK_UNLOCK_OFF_IN;
        	break;
        case 0x80:
        	if((new_Input & resul) == resul)
            	input = DOOR_CLOSED_IN;
        	else
        	break;
        default:
        	break;
        
        
      }
        
        ledToggle(6);
    }
    
    last_Input = new_Input;     
    val = input;
	
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
		new_Input = UNLOCK_ALL_IN;
	  
	}
	if(!(PTIH & 0x04)){
		new_Input = LOCK_ALL_IN;
	}
	if(!(PTIH & 0x08)){
		new_Input = CRASH_ON_IN;
	  
	}
	if(!(PTIH & 0x10)){
	   new_Input = CENTRAL_UNLOCK_IN;
	}
	if(!(PTIH & 0x20)){
	  new_Input = CENTRAL_LOCK_IN;
	}
	PIFH = PIFH | 0xFF;      //clear PTH Interupt Flags for the next round. Writing HIGH will clear the Interrupt flags

	processInput(new_Input);
	processState();	
} 

// Function for required delays
void MSDelay(unsigned int itime) {
  
  unsigned int i; 
  unsigned int j;
  
  for(i=0;i<itime;i++) {
    for(j=0;j<4000;j++);
  }
}