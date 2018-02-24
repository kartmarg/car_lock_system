/* 
 * This module provides functionality to sound the beeper at
 * a certain frequency on the Dragon21 board from Wytec. It uses 
 * the ECT channel 5 for timing the toggle of the speaker.
 *
 * Make sure the interrupt vector routine is properly hooked into
 * the interrupt vector table in file Monitor_linker.prm like this:
 *
 * VECTOR ADDRESS 0xFFE4  isr_tc5 
 *
 * IDE used: Metrowerks Codewarrior
 *
 * References: 
 *   Dragon12 example program Ex6.asm by Wayne Chu, Wytec Co.
 *   MC9S12DP256B Device User Guide, V02.14, Mar 5 2003, Motorola Inc.
 *   ECT_16B8C Block User Guide, V01.03, Jul 18 2002, Motorola Inc.
 *
 * Author: Joerg Friedrich
 * Copyright: FHT Esslingen, March 2006
 *
 * License: Gnu Lesser Public License (LGPL)
 *
 * $Id:$
 * $Rev:$
 *
 */

#include <mc9s12dp256.h>
#include "beeper.h"

/* Interrupt service routine for timer channel 5 */
interrupt void isr_tc5(void); 

/* Define "magic" numbers */
#define F3500HZ (3500 )  /* timer constant for 3500Hz */
#define TIMER_ON (0x80)  /* tscr1 value to turn ECT on */
#define TIMER_CH5 (0x20) /* bit position for channel 5 */
#define TCTL1_CH5 (0x0c) /* Mask corresponds to TCTL1 OM5, OL5 */
#define TCTL1_CH5_TOGGLE (0x04) /* TCTL1 OM5, OL5 Output toggle */
#define TCTL1_CH5_LOW (0x08)  /* TCTL1 OM5, OL5 Output zero */


static int frequency = F3500HZ;
	
/* Interrupt service routine for timer channel 5 */
interrupt 13 void isr_tc5(void) {

	/* increment timer count register (16 bits) */
	TC5 = TC5 + frequency; /* current count + increment = new count */

	/* clear interrupt flag (RTIF) */
  TFLG1 = TIMER_CH5; /* clear the interrupt: write a 1 to bit 5 */
} // end isr_tc5()



/*
 * Initialize the beeper hardware and interrupt (5) 
 *
 */
void initBeeper(void) {
  /* Timer master ON switch  */
	TSCR1 = TIMER_ON;
	/* Set channel 5 in "output compare" mode  */
	TIOS = TIOS | TIMER_CH5; /* bit 5 corresponds to channel 5 */
	/* Enable channel 5 interrupt */
  TIE = TIE | TIMER_CH5;  /* bit 5 corresponds to channel 5 */
  /* Set timer prescaler (bus clock : prescale factor)        */
  /* In our case: divide by 2^7 = 128. This gives a timer     */
  /* driver frequency of 187500 Hz or 5.3333 us time interval */
  TSCR2 = (TSCR2 & 0xf8) | 0x07;
}


/* 
 * Turn on beeper output.
 * Mask off OM5 OL5 bits, then set to OM5=0, OL5=1
 * Set frequency to desired value (bus clock is 24.0 MHz)
 * Bus clock is determined by crystal (4.0 MHz) and registers
 * at 0x034 and 0x035.
 * 
 */
void beeperOn(int freq) {
  if (freq > 0) {
    frequency = (int) (BUSFREQ/2/freq);
    TCTL1 = (TCTL1 & ~TCTL1_CH5) | TCTL1_CH5_TOGGLE;
  } 
  else {
    beeperOff(); 
  }
}


/*
 * Turn off beeper output.
 * Mask off OM5 OL5 bits, then set to OM5=1, OL5=0.
 *
 */
void beeperOff(void) {
  TCTL1 = (TCTL1 & ~TCTL1_CH5) | TCTL1_CH5_LOW;
}








