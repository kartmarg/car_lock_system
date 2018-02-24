/* 
 * A primitive CAN driver for the Dragon 12 board.
 *
 * $Id:$
 * $Rev:$
 *
 */
 
#include <hidef.h>      	/* common defines and macros */
#include <mc9s12dp256.h>	/* derivative information */

 
static unsigned char rdata[8];   /* for data received */
static unsigned char rid[4];
static unsigned char rdataLength;
static unsigned char newCANMessage = 0; 
 
interrupt 38 void CAN0_RECInterruptHandler(void) {

	if (CAN0RFLG_RXF==1) {
	    
	  rdata[0] = 0;
	  rdata[0] = CAN0RXDSR0;
	  //rdata[1] = CAN0RXDSR0;
	  //rdata[2] = CAN0RXDSR0;
	  //rdata[3] = CAN0RXDSR0;
	  //rdata[4] = CAN0RXDSR0;
	  //rdata[5] = CAN0RXDSR0;
	  //rdata[6] = CAN0RXDSR0;
	  //rdata[7] = CAN0RXDSR0;
 	
	  rdataLength = CAN0RXDLR;
	
	  rid[0] = CAN0RXIDR0;
	  rid[1] = CAN0RXIDR1;
	  rid[2] = CAN0RXIDR2;
	  rid[3] = CAN0RXIDR3;
	
	  CAN0RFLG_RXF = 1;    /* release receive buffer */
    CAN0CTL0_RXFRM = 0;  /* clear received frame flag */
    newCANMessage = 1;
	}
}


void initCAN(void) 
{
  asm sei                  /* make atomic       */
  CAN0CTL1_CANE = 1;		   /* enable msCAN      */

  CAN0CTL0_INITRQ = 1;    /* Start CAN Init Mode  */
  
  while (CAN0CTL1_INITAK==0) {
     /* wait for initialization to complete */    
  }
  
  CAN0CTL1_LISTEN = 0;    /* LISTEN=0, get out of Listen-only mode   */
  CAN0CTL1_CLKSRC = 0;    /* CLKSRC=0, use oscillator clock          */
  CAN0IDAC = 0x00;        /* four 16 bit filters, which we are not using anyways  */

  CAN0IDMR0 = 0xFF;       /* Ignore all acceptance register bits     */
  CAN0IDMR1 = 0xFF;       /* ...  */
  CAN0IDMR2 = 0xFF;
  CAN0IDMR3 = 0xFF;
  CAN0IDMR4 = 0xFF;
  CAN0IDMR5 = 0xFF;
  CAN0IDMR6 = 0xFF;
  CAN0IDMR7 = 0xFF;

  CAN0BTR0  = 0xC1;
  CAN0BTR1  = 0x49;

  CAN0CTL0_INITRQ = 0;     /* INITRQ=0, Leave Initialization mode */
  
  while(CAN0CTL1_INITAK==1) {
                           /* wait for end of initialization      */
	};   
  CAN0RIER |= 0x01;        /* Arm RxF, interrupt on  */
  asm cli;                 /* leave atomic mode */
  return;
}




int writeCAN(unsigned char* id, unsigned char* data, unsigned char plength) 
{
	if (plength > 7) {
	  return -1;
	}
	
	if ((CAN0TFLG & 0x07)==0) {
	  return -1;
	}
	
	CAN0TBSEL_TX0=1;		
	
	if (CAN0TBSEL_TX0==1) {
	 	  CAN0TBSEL=0x01;		   /* Select transmit buffer 1 */
	}
	
	CAN0TXIDR0 = id[0];	     /*                          */
	CAN0TXIDR1 = id[1];
	CAN0TXIDR2 = id[2];
	CAN0TXIDR3 = id[3];
	
	CAN0TXDSR0 = data[0];	  /* always write 8 byte */
	//CAN0TXDSR1 = data[1];   /* may write nonsense but no problem! */
//	CAN0TXDSR2 = data[2];
	//CAN0TXDSR3 = data[3];
//	CAN0TXDSR4 = data[4];
//	CAN0TXDSR5 = data[5];
//	CAN0TXDSR6 = data[6];
//	CAN0TXDSR7 = data[7];
	CAN0TXDLR = plength;		/* Data length register	*/
	CAN0TXTBPR = 0;			
	CAN0TFLG = 0x01;		    /* Set transmit flag */
	return 0;	
}
  

  
int readCAN(unsigned char* id, unsigned char* data, unsigned char *plength)
{
   char i = 0;
   
   if (!newCANMessage) {
     return -1;
   }
   
   newCANMessage = 0;
   
   //while (i < rdataLength && i < *plength) {
     //data[i] = rdata[i];
     //++i;
   //}
   data[0] = 0;
   data[0] = rdata[0];
   
   *plength = rdataLength;
   id[0] = rid[0];
   id[1] = rid[1];
   id[2] = rid[2];
   id[3] = rid[3];
   return 0; 
}