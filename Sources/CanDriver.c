/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     CanDriver.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   适用于MC9S12G128单片机平台
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "CanDriver.h"  

CANTX_MSG *CanTxBuf;                                             /* 待发送信息存储区 */
CANRX_MSG CanRxBuf[4];
                                           
#pragma CODE_SEG __NEAR_SEG NON_BANKED    

interrupt VectorNumber_Vcanrx void CANRXISR(void)     /* CAN接收中断 */
{                      
    INT8U  i = 0;
    INT16U idtemp = *((INT16U *)&CANRXIDR0);
    
    idtemp = idtemp >> 5;
    
    if((CANIDAC_IDHIT < 4) && (CANRXDLR == 8))      /* 标识符接收有效标志指示器正确且长度为8 */
    {
        i = CANIDAC_IDHIT;
        /* 装载至CAN接收缓存区 */
        CanRxBuf[i].id      = idtemp;
        CanRxBuf[i].data[0] = CANRXDSR0;
        CanRxBuf[i].data[1] = CANRXDSR1;
        CanRxBuf[i].data[2] = CANRXDSR2;
        CanRxBuf[i].data[3] = CANRXDSR3;
        CanRxBuf[i].data[4] = CANRXDSR4;
        CanRxBuf[i].data[5] = CANRXDSR5;
        CanRxBuf[i].data[6] = CANRXDSR6;
        CanRxBuf[i].data[7] = CANRXDSR7;
        CanRxBuf[i].len     = CANRXDLR;
        CanRxBuf[i].newdata = TRUE;
    }
    
    CANRFLG_RXF = 1;                                  /* 清除接收标志 */    
}

interrupt VectorNumber_Vcantx void CANTXISR(void)     /* CAN发送缓冲区空中断 */
{   
   INT8U i = 0;
   INT8U idle_ch = CanTxStatusCheck();                
   
   if(CanTxBuf->next != NULL)                         /* 仍有待发送数据*/
   {
       CanTxBuf = CanTxBuf->next; 
       CanLoadStdMsg(CanTxBuf);                       /* 装载数据 */
       CANTFLG = idle_ch;                             /* 开启发送 */
   }else                                              /* 数据发送完成 */
   {
       CANTIER = 0;                                   /* 清除所有发送中断 */
   }
   
}  
   
#pragma CODE_SEG DEFAULT
/*
************************************************************************************************
*  Function:         CAN_Init()
*  Description:      CAN初始化函数
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           CAN模块初始化(set Fcan = 250KHz, needed: Fosc = 4MHz)                
************************************************************************************************
*/
void Can_Init(void)
{  
    INT8U cnt = 0;
    CANCTL0 =     0b00000001;          
    /*              ||||||||__  bit0: INITRQ= 1, MSCAN in initialization mode
	                  |||||||___  bit1: SLPRQ = 0, The MSCAN doesn't enter sleep mode when CAN bus idle
	                  ||||||____  bit2: WUPE  = 0, The MSCAN ignores traffic on CAN
	                  |||||_____  bit3: TIME  = 0, Disable internal MSCAN timer 
	                  ||||______  bit4: SYNCH,     Synchronized Status(Read Only):0--not synchronized,1--synchronized;
	                  |||_______  bit5: CSWAI = 0, CAN module is not affected during wait mode;                                               
	                  ||________  bit6, RXACT,     Receiver Active Status(Read Only):0--transmitting or idle,1--receiving; 
	                  |_________  bit7, RXFRM,     Received Frame Flag(Read Only):0--No valid message received since clearing,1--receive a valid message;
    */
    while (!(CANCTL1_INITAK));	    /* Wait for initialization mode acknowledge */
            
    CANCTL1_CANE    = 1;            /* Enable MSCAN module */
    CANCTL1_CLKSRC  = 0;            /* Clock source is OSCCLK, CANCLK = 4MHz */

#ifdef CAN_LOOP_MODE
    CANCTL1_LOOPB   = 1;            /* Set to 1 for LoopBack Mode */
#else
    CANCTL1_LOOPB   = 0;     
#endif
    
    CANCTL1_LISTEN  = 0;            /* Not listen only mode */       
                                                                            
    CANBTR1_TSEG_10 = TSEG1;        /* Time Segment1 = TSEG1 +1; */
    CANBTR1_TSEG_20 = TSEG2;        /* Time Segment2 = TSEG2 +1; */ 
                                    /* Time Segment1 + Time Segment2 + 1 = time quantas; */
    CANBTR0_BRP     = BRP;          /* Prescaler = CANCLK/(Baud rate * time quantas) */
    CANBTR0_SJW     = SJW;          /* Synchronization jump width = 4 clock cycles */
    CANBTR1_SAMP    = 0;            /* One sample per bit */	 
    
    CANIDAC_IDAM    = 0b01;         /* acceptance filter mode: Four 16-bit acceptance filters */
    
    /* set acceptance filters for accepting and mask*/
    CANIDMR0        = CAN_MASK1;   
    CANIDMR1        = CAN_MASK2;       
    CANIDMR2        = CAN_MASK3;    
    CANIDMR3        = CAN_MASK4;    

    CANIDAR0        = CAN_ACCEPT1;
    CANIDAR1        = CAN_ACCEPT2;
    CANIDAR2        = CAN_ACCEPT3;
    CANIDAR3        = CAN_ACCEPT4;

    CANIDMR4        = CAN_MASK5;    
    CANIDMR5        = CAN_MASK6;    
    CANIDMR6        = CAN_MASK7;    
    CANIDMR7        = CAN_MASK8;    

    CANIDAR4        = CAN_ACCEPT5;
    CANIDAR5        = CAN_ACCEPT6;
    CANIDAR6        = CAN_ACCEPT7;
    CANIDAR7        = CAN_ACCEPT8;

    CANCTL0_INITRQ  = 0;            /* Exit initialization mode request */
    while (CANCTL1_INITAK);         /* Wait for normal mode             */
                   
    
    while(!(CANCTL0_SYNCH))         /* Wait for CAN synchronization     */
    {
        cnt++;        
        if(cnt > 100)
        {
            break;
        }         
    }
   
    CANRFLG_RXF     = 1;            /* Clear receiver flags             */
    CANRIER_RXFIE   = 1;            /* Enable Receive interrupt         */
  
}
/*
************************************************************************************************
*  Function:         CanLoadStdMsg()
*  Description:      CAN标准帧数据装载函数
*  Calls:            无
*  Input:            CAN_MSG *p_msg
*  Output:           无
*  Return:           无
*  Others:           输入参数p_msg为待装载信息的结构体首地址             
************************************************************************************************
*/
void CanLoadStdMsg(CANTX_MSG *p_msg)
{   
    INT8U i = 0;
    
    *(INT16U *)(&CANTXIDR0)= (p_msg->id) << 5;        /* 装载帧ID */
   
    if(p_msg->rtr)
    {
        CANTXIDR0 |= 0x10;                            /* 装载RTR */
    }
  
    for(i = 0; i < 8; i++) 
    {
        *(&CANTXDSR0 + i) = p_msg->data[i];           /* 装载帧数据 */
    }
    
    CANTXDLR  = p_msg->len;                           /* 装载帧数据字节数 */
                                                      
    CANTXTBPR = p_msg->prty;                          /* 装载帧优先级 */
} 
/*
************************************************************************************************
*  Function:         CanTxStatusCheck()
*  Description:      CAN发送状态检测
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           INT8U
*  Others:           函数返回CAN的当前可用发送通道: 0无可用通道,1为TX0可用,2为TX1可用,4为TX2可用                                                      
************************************************************************************************
*/
INT8U CanTxStatusCheck(void)
{
    
    INT8U i = 0;
    INT8U status = 0;
     
    status =  CANTFLG;
    
    if(status == 0)
    {
        return 0;                                     /* CAN IS BUSY */ 
    }  
    
    CANTBSEL =  status;                               /* 设置指定可用的发送缓冲区 */
    
    status = CANTBSEL;                                
 
    return  status;                                   /* 返回优先可用的发送缓冲区 */
  
}