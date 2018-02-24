/* 
 * A primitive CAN driver for the Dragon 12 board.
 *
 * $Id:$
 * $Rev:$
 *
 */
#ifndef _CANDRIV_
#define _CANDRIV_

/* Initialize the module on CAN0, using interrupts */
/* This does not define any acceptance filters, all is open */
void initCAN(void);

/* Write a message with identifier in id, data in data, and length bytes */
void writeCAN(unsigned char * id, unsigned char* data, unsigned char length);

/* Read a message; place the message id into id, the message d into data, */
/* and the message length into length */
int readCAN(unsigned char* id, unsigned char* data,  unsigned char* length);

#endif