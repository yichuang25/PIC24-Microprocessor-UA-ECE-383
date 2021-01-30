/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */
#include "pic24_all.h"
#include <stdio.h>

/** \file
Illustrate CAN transmit, receive. Uses a FIFO for RX receive, and
uses a standard data frame.
*/

/**
Clock config taken from the PIC24H FRM ECAN datasheet (DS70226B, Example 21-9),
Produces data rate of 1 Mbps assuming FCY = 40 MHz, quanta = 20, Prescale = 2.
**/

void configBaudECAN1(void) { //set baud rate
#if FCY == 40000000L
// FCAN = FCY = 40 MHz. TQ = 20. Prescale = 2, Data Rate = FCAN/(TQ * pre) = 40MHz/40 = 1 MHz.
  C1CTRL1bits.CANCKS = ECAN_FCAN_IS_FCY; // CANCKS = 1, sets FCAN = FCY = 40 MHz
//20 TQ for a bit time. 20 = Sync(1) + Seg1 (8) + Seg2 (6) + Prop seg (5)
  C1CFG2 = ECAN_NO_WAKEUP |
           ECAN_SAMPLE_3TIMES |      //sample three times at sample point
           ECAN_SEG1PH_8TQ |         //seg1 = 8 TQ
           ECAN_SEG2_PROGRAMMABLE |  //seg2 is programmable
           ECAN_SEG2PH_6TQ |         //seg2 = 6 TQ
           ECAN_PRSEG_5TQ;           //propagation delay segment = 5 TQ

  C1CFG1 = ECAN_SYNC_JUMP_4 |    //use maximum sync jump width
           ECAN_PRE_2x1;         //prescalers to 2x1
#else
#warning ECAN module not configured! Edit function configECAN1()
#endif
}

#define NUM_TX_BUFS  1   //reserve 1 for TX
#define NUM_BUFS    8   //make this a power of 2 for the alignment to work or enter alignment manually
ECANMSG msgBuf[NUM_BUFS] __attribute__((space(dma),aligned(NUM_BUFS*16)));

//base message ID
#define MSG_ID 0x7A0    //arbitrary choice for 11-bit messsage ID

//configure DMA transmit buffer
void configDMA0(void) {
  DMACS0 = 0;
  _DMA0IF = 0;
  DMA0PAD = (unsigned int) &C1TXD;
  DMA0REQ = DMA_IRQ_ECAN1TX;
  DMA0STA = __builtin_dmaoffset(msgBuf);
  DMA0CNT =   7;  //8 words in ECANMSG
  DMA0CON =   //configure and enable the module Module
    (DMA_MODULE_ON |
     DMA_SIZE_WORD |
     DMA_DIR_WRITE_PERIPHERAL |
     DMA_INTERRUPT_FULL |
     DMA_NULLW_OFF |
     DMA_AMODE_PERIPHERAL_INDIRECT |
     DMA_MODE_CONTINUOUS);
}

//configure DMA receive buffer
void configDMA1(void) {
  _DMA1IF = 0;
  DMA1PAD = (unsigned int) &C1RXD;
  DMA1REQ = DMA_IRQ_ECAN1RX;
  DMA1STA = __builtin_dmaoffset(msgBuf);
  DMA1CNT =  7; //8 words in ECANMSG
  DMA1CON =   //configure and enable the module Module
    (DMA_MODULE_ON |
     DMA_SIZE_WORD |
     DMA_DIR_READ_PERIPHERAL |
     DMA_INTERRUPT_FULL |
     DMA_NULLW_OFF |
     DMA_AMODE_PERIPHERAL_INDIRECT |
     DMA_MODE_CONTINUOUS);

}

#define RX_BUFFER_ID  15  //a value of 15 means to use a FIFO for RX

void configECAN1() {
  uint8_t u8_i;
  CHANGE_MODE_ECAN1(ECAN_MODE_CONFIGURE);
  configBaudECAN1();
  C1FCTRL = ECAN_FIFO_START_AREA_1 | ECAN_DMA_BUF_SIZE_8 ;
//use Filter 0 with Mask 0
  configRxFilterECAN1(0, MSG_ID, ECAN_MATCH_SID, RX_BUFFER_ID, 0);
  configRxMaskECAN1(0, 0x7FC, 0, 0);  //check all but last two bits
  clrRxFullOvfFlagsECAN1();  //clear all RX full, overflow flags.

//first 8 buffs must be configured as either TX or TX
  for (u8_i = 0; u8_i<8; u8_i++) {
    if (u8_i < NUM_TX_BUFS)
      configTxRxBufferECAN1(u8_i,ECAN_TX_BUFF,3);
    else
      configTxRxBufferECAN1(u8_i,ECAN_RX_BUFF,3);
  }
  configDMA0();    //do DMA config after ECAN has been initialized
  configDMA1();
  CHANGE_MODE_ECAN1(ECAN_MODE_NORMAL);
}

uint32_t rrot32(uint32_t u32_x) {
  if (u32_x & 0x1) {
    u32_x = u32_x >> 1;
    u32_x = u32_x | 0x8000;
  } else u32_x = u32_x >> 1;
  return u32_x;
}

int main (void) {
  uint32_t u32_out0, u32_out1, u32_in0, u32_in1;
  uint8_t rx_buff_id, u8_cnt;

  configBasic(HELLO_MSG);
  configECAN1();
  CHANGE_MODE_ECAN1(ECAN_MODE_LOOPBACK);  //loopback to ourself for a test.
  u32_out0 = 0xFEDCBA98;
  u32_out1 = 0x76543210;
  u8_cnt = 0;
  while (1) {
    DELAY_MS(500);
    msgBuf[0].data.u32[0] = u32_out0; //save in CAN message
    msgBuf[0].data.u32[1] = u32_out1;
    //format Buffer 0 for TX with SID=MSG_ID, data length = 8 bytes
    formatStandardDataFrameECAN(&msgBuf[0], MSG_ID+u8_cnt, 8);
    startTxECAN1(0);  //start transmission of buffer 0
    while (getTxInProgressECAN1(0)) {
      doHeartbeat(); //wait for transmission to end.
    }
    _DMA0IF = 1;
    DELAY_MS(10);  //delay for reception
    if (!_DMA1IF) {
      printf("Message ID 0x%X rejected by acceptance filter.\n",MSG_ID+u8_cnt);
    } else {
      _DMA1IF = 0;   //RX message accepted
      rx_buff_id = GET_FIFO_READBUFFER_ECAN1();
      u32_in0 = msgBuf[rx_buff_id].data.u32[0];
      u32_in1 = msgBuf[rx_buff_id].data.u32[1];
      printf("Rx Buff: %d. Msg ID: 0x%X, Out: 0x%lx%lx, In: 0x%lx%lx\n",
             rx_buff_id, msgBuf[rx_buff_id].w0.SID,u32_out0, u32_out1, u32_in0, u32_in1 );
      clrRxFullFlagECAN1(rx_buff_id);
    }
    u32_out0 = rrot32(u32_out0);
    u32_out1 = rrot32(u32_out1);
    u8_cnt++;
    if (u8_cnt == 8) u8_cnt = 0;
  }//end while
}//end main
