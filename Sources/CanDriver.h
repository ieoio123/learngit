/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     CAN.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __CANDRIVER_H
#define __CANDRIVER_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
/* Baud rate = CANCLK / ((BRP + 1) * ((TSEG1 + 1) + (TSEG2 + 1) + 1))*/
#define   SJW     2               /* Synchronization Jump Width (value between 1 and 4 Tq) */ 
#define   BRP     0               /* Baud Rate Prescaler (Value between 1 and 64) */
#define   TSEG1   10              /* Time segment one Bit Time (value between 1 and 16 Tq) */
                                  /* TSEG1 = PROP_SEG + PHASE_SEG1 */ 
#define   TSEG2   3               /* Time segment two Bit Time (value between 1 and 8 Tq) */
                                  /* TSEG2 = PHASE_SEG2 */
                                  
#define   CAN_ACCEPT1  0x88       
#define   CAN_MASK1    0x7
#define   CAN_ACCEPT2  0x0      
#define   CAN_MASK2    0xff       /* 0x450~0x45F, 0x460~0x46F  */
#define   CAN_ACCEPT3  0x80      
#define   CAN_MASK3    0x0   
#define   CAN_ACCEPT4  0x0   
#define   CAN_MASK4    0x1f       /* 0x400       */
#define   CAN_ACCEPT5  0x2   
#define   CAN_MASK5    0x0
#define   CAN_ACCEPT6  0x20     
#define   CAN_MASK6    0x1f       /* 0x011       */
#define   CAN_ACCEPT7  0xa   
#define   CAN_MASK7    0x0
#define   CAN_ACCEPT8  0x20   
#define   CAN_MASK8    0x1f       /* 0x051       */

/*********************************Function declaration****************************************/
void Can_Init(void);
void CanLoadStdMsg(CANTX_MSG *p_msg);
INT8U CanTxStatusCheck(void);

#endif