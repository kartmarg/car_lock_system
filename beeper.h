/* 
 * This module provides functionality to sound the beeper at
 * a certain frequency on the Dragon21 board from Wytec. It uses 
 * the ECT channel 5 for timing the toggle of the speaker.
 *
 *
 * The bus frequency is currently defined here, but should actually
 * taken from where the clock and reset module is initialized.
 * Since this is done in the debug monitor, we write it out here
 * again.
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
 * $Id:$
 * $Rev:$
 *
 */
#ifndef _beeperH
#define _beeperH

#define BUSFREQ 24000000


/* 
 * Initialize the beeper hardware and interrupt.
 * Has to be called before the beeper can be used.
 *
 */
void initBeeper(void);

/* 
 * Turn on beeper output with given frequency.
 * Beeper can be turned off with frequency = 0.
 *
 */
void beeperOn(int frequency);


/* 
 * Explicitly turn off beeper output 
 *
 */
void beeperOff(void);


#endif