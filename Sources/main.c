/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     main.c
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V2.0          Build this moudle          2012-5-12   Zeng Qunxin
**********************************************************************************************/
#include "INCLUDES.h"

/*
************************************************************************************************
*  Function:         main()
*  Description:      主函数
*  Calls:            App_InitManage();App_ProcessManage();
*  Input:            
*  Output:           
*  Return:           
*  Others:                             
************************************************************************************************
*/
void main(void) 
{ 
           
    INT16U i;
    INT16U j;
    BOOLEAN flag_init=FALSE;
    MCG_Init();
    SPI1_Init();
    
    DDRP_DDRP0 = 1;
    PTP_PTP0 = 1;
    
    //DDR0AD_DDR0AD0 =0;
   // DDR0AD_DDR0AD2 =0;
   // DDR0AD_DDR0AD1 =0;
    
    //DDR1AD_DDR1AD0 = 0;
    //DDR1AD_DDR1AD1 = 0;
      DDR0AD_DDR0AD7 = 1;
    //DDR1AD =0;
    
    IO_Init(); 
    ADC_Init();
    Can_Init();
    EnableInterrupts;                   /* 开中断         */
    for(;;)
    {
       _FEED_COP();                      /* feeds the dog  */ 
        for(i=0;i<1000;i++)              /* delay */
      {
          for(j=0;j<300;j++)
          {
          
          }
      }
      PTP_PTP0 =~ PTP_PTP0;                 
        
      PT0AD_PT0AD7 = 1;
      
      
      PT0AD_PT0AD7 = 0;
      
      
       Vol_Samp();
       MaxMinVol_LookUp(); 
       Can_DataLoad();
       CanRxManage();
       CanTxManage();  
       ResTemp_Samp();
      
    }
}