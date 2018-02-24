#include "hal.h"
#include "inputs.h"
#include "actuators.h" 
#include "ticker.h"
#include "candriver.h" //TODO check if this works
#include "canprocessing.h"
#include "lcddrv.h"
#include <stdio.h>

// The ID of each door is set following the design document
// as a numeric constant

#define DOOR_ID 1 // Front Left
//#define DOOR_ID 2 // Front Right
//#define DOOR_ID 3 // Rear Right
//#define DOOR_ID 4 // Rear Left
//#define DOOR_ID 5 // Trunk

// A numeric value is assigned to each state

#define THEFT_AVOIDANCE_ST 1
#define LOCKED_ST 2
#define UNLOCKED_ST 3
#define OPEN_ST 4
#define CLOSED_ST 5

// Define ON as 1 and OFF as 0b
#define ON 1
#define OFF 0

// Define servomotor positions
#define THEFT_SERVO 90
#define LOCKED_SERVO 0
#define UNLOCKED_SERVO -90

// Door IDs

#define FRONT_LEFT 1 // Front Left
#define FRONT_RIGHT 2 // Front Right
#define REAR_RIGHT 3 // Rear Right
#define REAR_LEFT 4 // Rear Left
#define TRUNK 5 // Trunk 

// A numeric value is assigned to each CAN message for 
// easier processing

#define CRASH_ON_CAN 1 
#define CRASH_OFF_CAN 2
#define SPEED_ON_CAN 3
#define SPEED_OFF_CAN 4
#define LIGHTS_ON_CAN 5
#define LIGHTS_OFF_CAN 6
#define CHILD_LOCK_ON_CAN 7
#define CHILD_LOCK_OFF_CAN 8
#define CENTRAL_LOCK_CAN 9
#define CENTRAL_UNLOCK_CAN 10
#define DOOR_EXT_OPEN_CAN 11
#define DOOR_INT_OPEN_CAN 12
#define DOOR_LOCK_UNLOCK_CAN 13
#define LOCK_ALL_CAN 14
#define UNLOCK_ALL_CAN 15
#define UNLOCKED_CAN 16
#define LOCKED_CAN 17
#define KEEP_ALIVE_CAN 18
#define THEFT_AVOIDANCE_ON_CAN 19
#define THEFT_AVOIDANCE_OFF_CAN 20

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
//#define DOOR_LOCK_UNLOCK_IN 13
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
#define DOOR_CLOSED_IN 26
#define KEEP_ALIVE_IN 27
#define DOOR_LOCK_UNLOCK_ON_IN 28
#define DOOR_LOCK_UNLOCK_OFF_IN 29

// Declaration of global variables
int Current_State = UNLOCKED_ST; // Initialized as unlocked

// DEclaration of flags

int Crash = 0;
int Speed = 0;
int Child_Lock = 0;
int Lights = 0;

int Doors_Opened = 0;

// Char for input debugging
unsigned char var[17];

//Here we define the Servo positions, they're not going to be used to the door states.
// 0 = Unlocked
// 1 = Locked 
// 2 = Theft Avoidance
int Servo_Position = 0;

// Defining door States where 0 is unlocked and 1 is locked and 2 Theft locked
int Door_States[5] = {0, 0, 0, 0, 0}; //the 5th door(TRUNK) will only have 0 or 1

int Open_Doors[5]= {0,0,0,0,0}; //If there´s a 0, the door is open, if 1, the door is closed

int cont=0; //This counter will let us close the doors, when reached a limit.

//////////////////// State Changes/Processing //////////////////////

// PROCESS INPUT
// Purpose: Reacts to any input (which are sent to an initial variable int input)
// and depending on each case, it switches to the correct state, using the Crash, 
// Speed, Child_Lock, Lights, and DOOR_ID flags/identifiers for further validation
// Initial values: int input
// Return values: none
void processInput(int input){
ledToggle(6);
	switch (input) {
		case CRASH_ON_IN:
			Crash = 1;
			Current_State = UNLOCKED_ST;
			if (DOOR_ID == 1)
				writeCrash(DOOR_ID, ON); //activate the CRASH in order to unlock the doors
			break;
			
		case CRASH_OFF_IN:
			Crash = 0;
			Current_State = UNLOCKED_ST;
			if (DOOR_ID == 1)
				writeCrash(DOOR_ID, OFF);
			break;
			
		case SPEED_ON_IN:
		  ledToggle(5);
			Speed = 1;
			writeSpeed(DOOR_ID, ON); //Indicator that speed has passed 10kmh
			break;
			
		case SPEED_OFF_IN:
			Speed = 0;
			writeSpeed(DOOR_ID, OFF);
			break;
			
		case LIGHTS_ON_IN:
			Lights = 1;
			break;
			
		case LIGHTS_OFF_IN:
			Lights = 0;
			break;
			
		case CHILD_LOCK_ON_IN:
			Child_Lock = 1;
			if (DOOR_ID == 1)
				writeChildLock(DOOR_ID, ON);
			break;
			
		case CHILD_LOCK_OFF_IN:
			Child_Lock = 0;
			if (DOOR_ID == 1)
				writeChildLock(DOOR_ID, OFF);
			break;
			
		case CENTRAL_LOCK_IN:
			Current_State = THEFT_AVOIDANCE_ST;
			Door_States[0]=2;
			if (DOOR_ID == 1)
				writeTheftAvoidance(DOOR_ID, ON);
			break;
			
		case CENTRAL_UNLOCK_IN:
			Current_State = UNLOCKED_ST;
			if (DOOR_ID == 1)
				writeUnlockAll(ON);
			break;
			
		case DOOR_EXT_OPEN_IN:
			if (Current_State != LOCKED_ST && Current_State != THEFT_AVOIDANCE_ST)
				Current_State = OPEN_ST;
			break;
			
		case DOOR_INT_OPEN_IN:
			if (Current_State != LOCKED_ST && Current_State != THEFT_AVOIDANCE_ST 
                && ((DOOR_ID != 3 && DOOR_ID != 4) && Child_Lock == 0) )				Current_State = OPEN_ST;
			break;
			

	  case DOOR_LOCK_UNLOCK_ON_IN:
	    if(Current_State != THEFT_AVOIDANCE_ST)
	      Current_State=UNLOCKED_ST;
	    break;
	  
	  case DOOR_LOCK_UNLOCK_OFF_IN:
	    if(Current_State != THEFT_AVOIDANCE_ST)
	      Current_State=LOCKED_ST;
	    break;
			
		case LOCK_ALL_IN:
			if (Current_State == UNLOCKED_ST){		  
				Current_State = LOCKED_ST;
				writeLockAll(1);
			}
			break;
			
		case UNLOCK_ALL_IN:
			if (Current_State == LOCKED_ST && Current_State != THEFT_AVOIDANCE_ST) {	  
				Current_State = UNLOCKED_ST;
			  writeUnlockAll(1);
			}
			else if( Current_State == THEFT_AVOIDANCE_ST)
            	Current_State= THEFT_AVOIDANCE_ST;
			break;
			
		case DOOR_CLOSED_IN:
        	if(Current_State != CLOSED_ST)
        		Current_State = CLOSED_ST;
        	  Open_Doors[0]=0;
			break;
			
		case FRONT_RIGHT_UNLOCKED_IN:
			// Front Left is unlocked
			Door_States[1] = 0;
			break;
			
		case FRONT_RIGHT_LOCKED_IN:
			// Front Left is locked
			Door_States[1] = 1;
			break;
			
		case REAR_LEFT_UNLOCKED_IN:
			// Rear Left is unlocked
			Door_States[2] = 0;
			break;
			
		case REAR_LEFT_LOCKED_IN:
			// Rear Left is locked
			Door_States[2] = 1;
			break;
			
		case REAR_RIGHT_UNLOCKED_IN:
			// Rear Right is unlocked
			Door_States[3] = 0;
			break;
			
		case REAR_RIGHT_LOCKED_IN:
			// Rear Right is locked
			Door_States[3] = 1;
			break;
		
		case TRUNK_UNLOCKED_IN:
			// Trunk is unlocked
			Door_States[4] = 0;
			break;
			
		case TRUNK_LOCKED_IN:
			// Trunk is locked
			Door_States[4] = 1;
			break;
		
		default:
			// No change
			Current_State = Current_State;
			break;
	}
}

void processState(){
	int door_id;
	
	switch (Current_State){
		
		case THEFT_AVOIDANCE_ST:
			lockPosition(THEFT_SERVO);
			break;
			
		case LOCKED_ST:
			lockPosition(LOCKED_SERVO);
			// If door is not FL door, it must inform the main door that it is 
			// unlocked
			break;
			
		case UNLOCKED_ST:
			lockPosition(UNLOCKED_SERVO);
			// If door is not FL door, it must inform the main door that it is 
			// locked
			if(Speed==1 ){ //if we're going faster than 10km/h and the current state is unlocked(NOT OPEN), then lock doors
				if(cont > 200){ //Counter added instead of a delay
          Door_States[0]=1;
          Current_State= LOCKED_ST;
					cont=0;
				}
				cont++;
        	} 
			else
				Door_States[0] = 0;
			break;
			
		case OPEN_ST:
			Open_Doors[0] = 1;
			break;
			
		case CLOSED_ST:
			Open_Doors[0] = 0;
			Doors_Opened=0;
       beeperOff();
       Current_State = UNLOCKED_ST;
			break;
	}
	
	// Turns ON/OFF Leds depending on which doors are Open/Closed
	for (door_id = 0; door_id <= 4; door_id++){
		// If Door i is opened, the led must be turned ON
		if (Open_Doors[door_id] == 1)
			ledOn(door_id);
		// If Door i is closed, the led must be turned OFF
		else 
			ledOff(door_id);
	}
	
	
	//PROCESS FOR THE BEEPER
	
	if (Lights == 1){   //podria hacerse un and the puerta izq abierta y lights 1
		// If the Front Left door is opened (Driver's door) and the lights on, then beeper state must be ON
		if (Open_Doors[0] == 1) {
			beeperOn(90000);// TODO: checar este pedo, necesita una frecuencia adentro, creo.
		}
		// If the Front left door is locked, the beeper state can be activated if any door is open and speed>10
	}
	else if(Speed==1) {	
		for(door_id = 0; door_id <= 4; door_id++){
        	if(Open_Doors[door_id]==1)
        		  Doors_Opened = 1;
		}
			
		if (Doors_Opened == 1) //call function returns 1 if a door is open
				beeperOn(90000);
		else
		  	beeperOff();// TODO: checar este pedo del beeper
	}else 
	  //Doors_Opened=0;
		beeperOff();
}

		
void refreshDisplay(){
	char state[17];
	
	switch(Current_State){
		case THEFT_AVOIDANCE_ST:
		    writeLine("State: Theft   ", 0);
		    break;
		case LOCKED_ST:
		    writeLine("State: Locked  ", 0);
		    break;
		case UNLOCKED_ST:
		    writeLine("State: Unlocked", 0);
		    break;
		case OPEN_ST:
		    writeLine("State: Open     ", 0);
		    break;
		case CLOSED_ST:
		    writeLine("State: Closed   ", 0);
		    break;
		default:
		    break;
	}
	
	sprintf(var, "Sp:%i CL:%i L:%i", Speed, Child_Lock, Lights);
	writeLine(var, 1);

	
}