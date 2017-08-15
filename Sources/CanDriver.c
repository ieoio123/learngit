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
*Descriprion :   ������MC9S12G128��Ƭ��ƽ̨
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "CanDriver.h"  

CANTX_MSG *CanTxBuf;                                             /* ��������Ϣ�洢�� */
CANRX_MSG CanRxBuf[4];
                                           
#pragma CODE_SEG __NEAR_SEG NON_BANKED    

interrupt VectorNumber_Vcanrx void CANRXISR(void)     /* CAN�����ж� */
{                      
    INT8U  i = 0;
    INT16U idtemp = *((INT16U *)&CANRXIDR0);
    
    idtemp = idtemp >> 5;
    
    if((CANIDAC_IDHIT < 4) && (CANRXDLR == 8))      /* ��ʶ��������Ч��־ָʾ����ȷ�ҳ���Ϊ8 */
    {
        i = CANIDAC_IDHIT;
        /* װ����CAN���ջ����� */
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
    
    CANRFLG_RXF = 1;                                  /* ������ձ�־ */    
}

interrupt VectorNumber_Vcantx void CANTXISR(void)     /* CAN���ͻ��������ж� */
{   
   INT8U i = 0;
   INT8U idle_ch = CanTxStatusCheck();                
   
   if(CanTxBuf->next != NULL)                         /* ���д���������*/
   {
       CanTxBuf = CanTxBuf->next; 
       CanLoadStdMsg(CanTxBuf);                       /* װ������ */
       CANTFLG = idle_ch;                             /* �������� */
   }else                                              /* ���ݷ������ */
   {
       CANTIER = 0;                                   /* ������з����ж� */
   }
   
}  
   
#pragma CODE_SEG DEFAULT
/*
************************************************************************************************
*  Function:         CAN_Init()
*  Description:      CAN��ʼ������
*  Calls:            ��
*  Input:            ��
*  Output:           ��
*  Return:           ��
*  Others:           CANģ���ʼ��(set Fcan = 250KHz, needed: Fosc = 4MHz)                
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
*  Description:      CAN��׼֡����װ�غ���
*  Calls:            ��
*  Input:            CAN_MSG *p_msg
*  Output:           ��
*  Return:           ��
*  Others:           �������p_msgΪ��װ����Ϣ�Ľṹ���׵�ַ             
************************************************************************************************
*/
void CanLoadStdMsg(CANTX_MSG *p_msg)
{   
    INT8U i = 0;
    
    *(INT16U *)(&CANTXIDR0)= (p_msg->id) << 5;        /* װ��֡ID */
   
    if(p_msg->rtr)
    {
        CANTXIDR0 |= 0x10;                            /* װ��RTR */
    }
  
    for(i = 0; i < 8; i++) 
    {
        *(&CANTXDSR0 + i) = p_msg->data[i];           /* װ��֡���� */
    }
    
    CANTXDLR  = p_msg->len;                           /* װ��֡�����ֽ��� */
                                                      
    CANTXTBPR = p_msg->prty;                          /* װ��֡���ȼ� */
} 
/*
************************************************************************************************
*  Function:         CanTxStatusCheck()
*  Description:      CAN����״̬���
*  Calls:            ��
*  Input:            ��
*  Output:           ��
*  Return:           INT8U
*  Others:           ��������CAN�ĵ�ǰ���÷���ͨ��: 0�޿���ͨ��,1ΪTX0����,2ΪTX1����,4ΪTX2����                                                      
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
    
    CANTBSEL =  status;                               /* ����ָ�����õķ��ͻ����� */
    
    status = CANTBSEL;                                
 
    return  status;                                   /* �������ȿ��õķ��ͻ����� */
  
}