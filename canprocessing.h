void writeKeepAlive();
void writeLocked (int door_id);
void writeUnlocked (int door_id);
void writeUnlockAll (int door_id);
void writeTheftAvoidance (int door_id, int theft_avoidance_state);
void writeChildLock (int door_id, int child_lock_state);
void writeSpeed (int door_id, int speed_state);
void writeCrash (int door_id, int crash_state);
int readCANMessages();
