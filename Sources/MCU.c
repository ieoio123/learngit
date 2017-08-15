/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     MCU.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   适用于MC9S12G128单片机平台
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include <hidef.h>      
#include "derivative.h"      /* derivative-specific definitions */
#include "MCU.h"
#include "TYPEDEFINE.h"
/*
************************************************************************************************
*  Function:         MCG_Init()
*  Description:      时钟初始化
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           Setup clock module for PLL Engaged using externals XTAL 
*                    VCOCLK - 48MHz, Core Clk - 48MHz, Bus Clk - 24MHz                 
************************************************************************************************
*/
void MCG_Init(void)
{
    CPMUSYNR    = 0b00000101;
    /*              ||||||||_
	                  |||||||__\
	                  ||||||___ |
	                  |||||____ |bit5:0, SYNDIV  = 0b000101, set Fvco = 48MHz, Fref = Fvco / ((0b000101 + 1) * 2 );
	                  ||||_____ |
	                  |||______/
	                  ||_______
	                  |________> bit7:6, VCOFRQ  = 0b00,     set Fvco range : 32MHz <= Fvco <= 48MHz;
    */              
    CPMUPOSTDIV = 0b00000000;
    /*              ||||||||_
	                  |||||||__\
	                  ||||||___ |
	                  |||||____ |bit4:0, POSTDIV = 0b00000,  set Fpll = 48MHz. Fpll = Fvco / (0b00000 + 1);
	                  ||||_____/
	                  |||______
	                  ||_______
	                  |________ 
    */ 	
    CPMUREFDIV  = 0b01000000;
    /*              ||||||||_
	                  |||||||__\
	                  ||||||___ |
	                  |||||____ |bit4:0, REFDIV  = 0b0000,   set Fref = 4MHz. Fref = Fosc / (0b0000 + 1);
	                  ||||_____/
	                  |||
	                  ||_______
	                  |________> bit7:6, REFFRQ  = 0b01,     set Fref range : 2MHz < Fvco <= 6MHz;  
    */
    CPMUOSC     = 0b10000000;                                           
    /*              ||||||||_
	                  |||||||__
	                  ||||||___   
	                  |||||____ 
	                  ||||_____
	                  |||______  bit5, OSCPINS_EN(read), Osc Pins Enable Bit:0--pins exclusively reserved for oscillator,1--not reserved
	                  ||_______
	                  |________  bit7, OSCE         = 1, External oscillator is enabled; 
    */
    while(!CPMUFLG_UPOSC || !CPMUFLG_LOCK); /* wait for ext osc to stabilize and pll to lock */            
	
    CPMUFLG     = 0b11111111;				 
    /*              ||||||||_  bit0, UPOSC  = 1(read),  OscStatus Bit:0--osc is not qualified by the PLL,1--osc is qualified by the PLL;
	                  |||||||__  bit1, OSCIF  = 1(clear), clear Oscillator Interrupt Flag;
	                  ||||||___  bit2, ILAF      (read),  clear Illegal Address Reset Flag;
	                  |||||____  bit3, LOCK      (read),  PLL lock condition:0--PLL unlocked(Fpll = Fvco / 4),1--PLL locked(Fpll = Fvco / (POSTDIV + 1));  
	                  ||||_____  bit4, LOCKIF = 1(clear), clear PLL Lock Interrupt Flag;
	                  |||______  bit5, LVRF   = 1(clear), clear Low Voltage Reset Flag;
	                  ||_______  bit6, PORF   = 1(clear), clear Power on reset Flag;
	                  |________  bit7, RTIF   = 1(clear), clear RTI time-out Interrupt Flag; 
    */
    CPMUCOP     = 0b01000110; 
    /*              ||||||||_  
	                  |||||||__\ bit2:0, CR[2:0] = 0b110, enable COP, select the COP time-out rate is 2^23(COPCLK); 
	                  ||||||___/  
	                  |||||____  
	                  ||||_____
	                  |||______  bit5, WRTMASK = 0, Write of bit7,2:0 has an effect with this write of CPMUCOP(write only);
	                  ||_______  bit6, RSBCK   = 1, Stops COP and RTI counters when it is in Action BDM mode; 
	                  |________  bit7, WCOP    = 0,  Normal COP operation; 
    */ 
    CPMUCLKS    = 0b10000011;
    /*              ||||||||_  bit0, COPOSCSEL0 = 1, COP clcok source is OSCCLK, Fcopclk = Fosc = 4MHz;
	                  |||||||__  bit1, RTIOSCSEL  = 1, RTI clock source is OSCCLK, Frticlk = Fosc = 4MHz;
	                  ||||||___  bit2, PCE        = 0, COP stops running during Pseudo Stop Mode  
	                  |||||____  bit3, PRE        = 0, RTI stops running during Pseudo Stop Mode;
	                  ||||_____  bit4, COPOSCSEL1 = 0, COP clock source defined by COPOSCSEL0;
	                  |||______
	                  ||_______  bit6, PSTP       = 0, Oscillator is disabled in Stop Mode(Full Stop Mode); 
	                  |________  bit7, PLLSEL     = 1, System clocks are derived from PLLCLK, Fbus = Fpll / 2 = 8MHz; 
	  */ 
    CPMURTI     = 0b11010011;
    /*              ||||||||_  
	                  |||||||__\   
	                  ||||||___ |bit3:0, RTR[3:0] = 0b0011\  
	                  |||||____/                           |
	                  ||||_____                            |RTI Frequency Divide Rates is 2 * 10^5, Frti = Fcopclk / (2 * 10^5) = 20Hz; 
	                  |||______\ bit6:4, RTR[6:4] = 0b101 /
	                  ||_______/  
	                  |________  bit7,   RTCDEC = 1, Decimal based divider value of RTI; 
    */        
}
/*
************************************************************************************************
*  Function:         MCG_DelayMs()
*  Description:      延时函数
*  Calls:            无
*  Input:            INT16U time_ms
*  Output:           无
*  Return:           无
*  Others:           延时time_ms个单位的毫秒时间                
************************************************************************************************
*/
void MCG_DelayMs(INT16U time_ms) 
{  
    INT16U i;
    INT16U j;
    
    for(i = 0; i < time_ms; i++)
    {
        for(j = 0; j < DELAY_1MS; j++)
        {
            _asm NOP;
        }
    }
}