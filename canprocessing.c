#include "hal.h"
#include "inputs.h"
#include "actuators.h" 
#include "ticker.h"
#include "candriver.h" //TODO check if this works

/////// File to process the CAN messages////////

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

// Declare global variables that are used when writing CAN messages
unsigned char ID[4] = {0,0,0,0};
unsigned char Data[8] = {0,0,0,0,0,0,0,0};
unsigned char Length = 1;

// Declare variables for data that the system will receive
unsigned char Received_ID[4] = {0,0,0,0};
unsigned char Received_Data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char Received_Length = 0;

char message_can[17];


////////////////// Write CAN Messages ///////////////////

void writeKeepAlive(int door_id) {    //This function will be periodically sent to the other boards in order to check failures in communication.
	ID[0] = door_id;
	Data[0] = KEEP_ALIVE_CAN;
	writeCAN(ID, Data, Length);	
}

void writeLocked (int door_id) {	//Generic function that indicates the Center Control that the door lock is closed.
	ID[0] = door_id;
	Data[0] = LOCKED_CAN;
	writeCAN(ID, Data, Length);	
}

void writeUnlocked (int door_id) {	//Generic function that indicates the Center Control that the door lock is open.
	ID[0] = door_id;
	Data[0] = UNLOCKED_CAN;
	writeCAN(ID, Data, Length);		
}

void writeUnlockAll (int door_id) { //Void that will unlock all the doors.
	ID[0] = door_id;
	Data[0] = UNLOCK_ALL_CAN;
	ledToggle(7);
	writeCAN(ID, Data, Length);
}

void writeLockAll (int door_id) { //Void that will unlock all the doors.
	ID[0] = door_id;
	Data[0] = LOCK_ALL_CAN;
	writeCAN(ID, Data, Length);
}



void writeTheftAvoidance (int door_id, int theft_avoidance_state) {// Indicates the other doors to enter in the Theft avoidance state.
	ID[0] = door_id;
	if (theft_avoidance_state == 1)
		Data[0] = THEFT_AVOIDANCE_ON_CAN;
	if (theft_avoidance_state == 0)
		Data[0] = THEFT_AVOIDANCE_OFF_CAN;
	writeCAN(ID, Data, Length);	
}

void writeChildLock (int door_id, int child_lock_state) {//Master sents to the rear doors that child lock feature is activated.
	ID[0] = door_id;
	if (child_lock_state == 1)
		Data[0] = CHILD_LOCK_ON_CAN;
	if (child_lock_state == 0)
		Data[0] = CHILD_LOCK_OFF_CAN;
	writeCAN(ID, Data, Length);	
}

void writeSpeed (int door_id, int speed_state) { //The other doors will know that speed is over 10 km/h, in order to take the respective decisions (lock the doors).
	ID[0] = door_id;
	if (speed_state == 1)
		Data[0] = SPEED_ON_CAN;
	if (speed_state == 0)
		Data[0] = SPEED_OFF_CAN;
	writeCAN(ID, Data, Length);	
}

void writeCrash (int door_id, int crash_state) { // If a crash is detected, a message will be sent in order to unlock all the doors.
	ID[0] = door_id;
	if (crash_state == 1)
		Data[0] = CRASH_ON_CAN;
	if (crash_state == 0)
		Data[0] = CRASH_OFF_CAN;
	writeCAN(ID, Data, Length);	
}

/////////////////// Read CAN Messages ///////////////////

int readCANMessages(){
	
	int message_ID = -1;
	
	// If a message is received readCAN returns a 0
	if (readCAN(Received_ID, Received_Data, &Received_Length) == 0) {
		
		switch (Received_ID[0]) {
		
			case FRONT_LEFT:
				switch (Received_Data[0]){
					case KEEP_ALIVE_CAN:
						// TODO keep alive logic???
						break;
						
					case UNLOCK_ALL_CAN:
						message_ID = UNLOCK_ALL_IN;
						break;
						
					case THEFT_AVOIDANCE_ON_CAN:
						message_ID = THEFT_AVOIDANCE_ON_IN;
						break;
					
					case THEFT_AVOIDANCE_OFF_CAN:
						message_ID = THEFT_AVOIDANCE_OFF_IN;
						break;
						
					case CHILD_LOCK_ON_CAN:
						message_ID = CHILD_LOCK_ON_IN;
						break;
						
					case CHILD_LOCK_OFF_CAN:
						message_ID = CHILD_LOCK_OFF_IN;
						break;
					
					case SPEED_ON_CAN:
						message_ID = SPEED_ON_IN;
						break;
						
					case SPEED_OFF_CAN:
						message_ID = SPEED_OFF_IN;
						break;
						
					case CRASH_ON_CAN:
						message_ID = CRASH_ON_IN;
						break;
						
					case CRASH_OFF_CAN:
						message_ID = CRASH_OFF_IN;
						break;
						
					default:
						message_ID = -1;
						break;
				}
				break;
			
			case FRONT_RIGHT:
				switch (Received_Data[0]){
					case UNLOCKED_CAN:
						message_ID = FRONT_RIGHT_UNLOCKED_IN;
						break;
						
					case LOCKED_CAN:
						message_ID = FRONT_RIGHT_LOCKED_IN;
						break;
					
					default:
						message_ID = -1;
						break;
				}
				break;
				
			case REAR_RIGHT:
				switch (Received_Data[0]){
					case UNLOCKED_CAN:
						message_ID = REAR_RIGHT_UNLOCKED_IN;
						break;
						
					case LOCKED_CAN:
						message_ID = REAR_RIGHT_LOCKED_IN;
						break;
					
					default:
						message_ID = -1;
						break;
				}
				break;

			case REAR_LEFT:
				switch (Received_Data[0]){
					case UNLOCKED_CAN:
						message_ID = REAR_LEFT_UNLOCKED_IN;
						break;
						
					case LOCKED_CAN:
						message_ID = REAR_LEFT_LOCKED_IN;
						break;
					
					default:
						message_ID = -1;
						break;
				}
				break;
				
			case TRUNK: 
				switch (Received_Data[0]){
					case UNLOCKED_CAN:
						message_ID = TRUNK_UNLOCKED_IN;
						break;
						
					case LOCKED_CAN:
						message_ID = TRUNK_LOCKED_IN;
						break;
					
					default:
						message_ID = -1;
						break;
				}
				break;
			
			default:
				message_ID = -1;
				break;
		}
	
	  sprintf(message_can, "DR: %i CD: %i", Received_ID[0], message_ID);
	  writeLine(message_can, 1);
	  
		return message_ID;
		
	}
	
	return -1;
}