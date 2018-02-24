 /* $Rev:$
 *
 */

#include <mc9s12dp256.h>
#include "pwm.h"



/*
 * Initialize the pulse width modulator for port PP7
 * which is used to control the servo. The servo is controlled via
 * a positive pulse between about 0.8 ms and 2.2 ms, with 2.5 ms being
 * neutral. The pulse frequency is not really critical, 20 to 100 ms seems
 * to work quite well.
 *
 */
void openPWM(void) {

    PWMPRCLK = 0x20; /* Prescaler for clock B set to 128 -> 24 MHz / 128 = 187500 Hz = 41,7 ns */
    PWMCTL   = 0x80; /* Concatenate channels 6 and 7 to form one 16 bit PWM */
    PWMSCLB  = 0x20; /* Scaler for clock TSB set to 32: clock period TSB = 2 · 32 · 41,7 ns = 10,7 us */
    PWMCLK   = 0xC0; /* Channel 7 uses TSB as clock signal */
    PWMPOL   = 0x80; /* output goes high when duty count is reached */
    PWMPER6  = 0x07; /* we want a pulse period of 20 ms */
    PWMPER7  = 0x53;
    PWMDTY6  = 0;    /* 50 == 0,5 ms, 230 == 2,3 ms, 140 == 1,5 ms */     
    PWMDTY7  = 140;
    PWMCAE   = 0x00;
    PWME = PWME | 0xC0;  /* enable PWM channel 7  */

}


/*
 * Set the servo position from -90 to +90 degrees, 0 is neutral.
 *
 */
void positionServo(int position) {
  if (position < -90) position = 90;
  if (position > 90) position = 90;
  PWMDTY7 = 140 + position;
  PWMCNT7 = 0;
  PWMCNT6 = 0; 
}