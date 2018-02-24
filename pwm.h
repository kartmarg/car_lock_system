/* Routines to control a servo (JR/Futaba) from Port pn PP7.
 * 
 * Author: Joerg Friedrich
 * Copyright: HS Esslingen, September 2008
 *
 * License: Gnu Lesser Public License (LGPL)
 *
 * $Id:$
 * $Rev:$
 *
 */
#ifndef _pwmH
#define _pwmH


/*
 * Initialize the pulse width modulator for port PP7
 * which is used to control the servo. The servo is controlled via
 * a positive pulse between about 0.8 ms and 2.2 ms, with 2.5 ms being
 * neutral. The pulse frequency is not really critical, 20 to 100 ms seems
 * to work quite well.
 *
 */
void openPWM(void);

/*
 * Set the servo position from -90 to +90 degrees, 0 is neutral.
 *
 */
void positionServo(int position);


#endif