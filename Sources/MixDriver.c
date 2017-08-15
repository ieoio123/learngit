/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     MixDriver.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   适用于MC9S12G128单片机平台,外部晶振4MHz
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "MixDriver.h"
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
}
void Rtc500ms(void)
{
    CPMURTI =     0b11111001;                           /* 设置500ms定时 */
    /*              ||||||||_  
	                  |||||||__\   
	                  ||||||___ |bit3:0, RTR[3:0] = 0b0001\  
	                  |||||____/                           |
	                  ||||_____                            |RTI Frequency Divide Rates is 2 * 10^6, Frti = Fcopclk / (2 * 10^6) ≈ 2Hz; 
	                  |||______\ bit6:4, RTR[6:4] = 0b111 /
	                  ||_______/  
	                  |________  bit7,   RTCDEC = 1, Decimal based divider value of RTI; 
    */  
    CPMUFLG_RTIF = 1;
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
        for(j = 0; j < TICK_DELAY_1MS; j++)
        {
            _asm NOP;
        }
    }
}
/*
************************************************************************************************
*  Function:         GPIO_Init()
*  Description:      模拟PWM的IO口初始化
*  Calls:            无
*  Input:            无
*  Output:           无 
*  Return:           无
*  Others:           无         
************************************************************************************************
*/
void IO_Init(void)
{
    //DDRJ_DDRJ5 = 1;
    //PTJ_PTJ5   = 0;
    //#if(0)
    //PTJ_PTJ5   = 0;
    //#endif
    /* Ltc6803-1使能脚 */
    DDRJ_DDRJ3 = 1;
    PTJ_PTJ3 = 1;
    /* 供电控制脚 */ 
    //DDRJ_DDRJ6 = 1;
    //PTJ_PTJ6 = 0;    
    //DDRJ_DDRJ7 = 1;
    //PTJ_PTJ7 = 0;  
}
/*
************************************************************************************************
*  Function:         FAN_Ctrl()
*  Description:      风扇控制
*  Calls:            无
*  Input:            无
*  Output:           无 
*  Return:           无
*  Others:           无         
************************************************************************************************
*/
void FAN_Ctrl(BOOLEAN status)
{
    FANDATA = status;   
}
/*
************************************************************************************************
*  Function:         ADCInit()
*  Description:      ADC初始化
*  Calls:            无
*  Input:            无
*  Output:           无 
*  Return:           无
*  Others:           无         
************************************************************************************************
*/
void ADC_Init(void)
{
    ATDCTL1     = 0b00111111;
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ |bit3:0,External Trigger Channel Select(select AN11);   
	                  |||||_____/ 
	                  ||||______  bit4, The internal sample capacitor is discharged before sampling the channel;
	                  |||_______
	                  ||________> bit6:5, 10-bit A/D conversion results;
	                  |_________  bit7, select the external trigger source to AN0~AN11; 
    */
    ATDCTL3     = 0b10001011;
    /*              ||||||||__
	                  |||||||___> bit1:0, ATD Freezes immediatey in Freeze Mode(Breakpoint);  
	                  ||||||____  bit2, Conversion results are placed in the sequence up to the sequence length; 
	                  |||||_____  
	                  ||||______\  
	                  |||_______ |bit6:3, set the number of conversion per sequence is 1(inital);
	                  ||________/
	                  |__________ bit7, Right justified data in the result registers; 
    */    
    ATDCTL4     = 0b01100100;
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ | 
	                  |||||_____ |bit4:0, set Fatdclk = Fbus / (2 * (0b00100 + 1)) = 800KHz; 
	                  ||||______/  
	                  |||_______ 
	                  ||________\
	                  |_________/ bit7:5, ATD sample time is 4 units of ATD Clock Cycles,Tad = 4 * 1 /Fatdclk = 5us;   
    */
    ATDCTL5     = 0b00000000;
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ |bit3:0, the channel to be examined is AN0(inital);  
	                  |||||_____/ 
	                  ||||______  bit4, Samled only one channel;
	                  |||_______  bit5, Single conversion sequence;
	                  ||________  bit6, Special channel conversions disabled;
	                  |_________
    */
} 
/*
************************************************************************************************
*  Function:         AdcConvert()
*  Description:      ADC转换
*  Calls:            无
*  Input:            INT8U ch
*  Output:           无 
*  Return:           INT16U
*  Others:           输入参数:ch为AD采样的通道值, 
*                    函数返回AD转换的结果         
************************************************************************************************
*/ 
INT16U ADC_Convert(INT8U ch)
{   
    ATDSTAT0 &= 0xF0;                                          /* clear sequence complete flag */
    ATDCTL5   = ch;                                            /* set the channel to be examined */   

    while(!ATDSTAT0_SCF);                                      /* wait sequence complete */    
    
    return ATDDR0;                                             /* store the conersion results */   
}
/*
************************************************************************************************
*  Function:         SPI1_Init()
*  Description:      SPI1模块初始化函数
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           SPI1模块时钟管脚和工作模式初始化(set Fspi = 1MHz, needed: Fbus = 24MHz)                
************************************************************************************************
*/
void SPI1_Init(void)
{
    static BOOLEAN sSPII_first_init = TRUE;
    
    if(sSPII_first_init)
    {     
        sSPII_first_init = FALSE;
        
        SPI1CR1     = 0b01011100;
        /*              ||||||||_  bit0, LSBFE = 0, Data is transferred most significant bit first;
    	                  |||||||__  bit1, SSOE  = 1, /S/S is slave select output;
    	                  ||||||___  bit2, CPHA  = 1, Sampling of data occurs at even edges (2,4,6,...) of the SCK clock;
    	                  |||||____  bit3, CPOL  = 1, Active-low SPI clock (idles high);
    	                  ||||_____  bit4, MSTR  = 1, SPI is in master mode;
    	                  |||______  bit5, SPTIE = 0, SPTEF interrupt disabled;
    	                  ||_______  bit6, SPE   = 1, SPI enable;
    	                  |________  bit7, SPIE  = 0, SPI interrupts disabled;
        */              
        SPI1CR2     = 0b00000000;
        /*              ||||||||_  bit0, SPC0    = 0, SPI Pin Mode is Normal Mode;
    	                  |||||||__  bit1, SPISWAI = 0, SPI clock operates normally in wait mode;
    	                  ||||||___   
    	                  |||||____  bit3, BIDIROE = 0, Output buffer disabled in the Bidirectional Mode;
    	                  ||||_____  bit4, MODFEN  = 1, /S/S is slave select output;
    	                  |||______   
    	                  ||_______  bit6, XFRW    = 0, 8-bit Transfer Width;
    	                  |________  
        */  
        SPI1BR      = 0b00100010; 
        /*              ||||||||_  
    	                  |||||||__\  
    	                  ||||||___/ bit2:0, SPR  = 0b010;
    	                  |||||____  
    	                  ||||_____\  
    	                  |||______/ bit6:4, SPPR = 0b010;Fspi = Fbus/((0b010 + 1) * 2^(0b010 + 1)) = 1MHz;  
    	                  ||_______  
    	                  |________  
        */
        SPI1CR1_SPE = 1;         /* SPI enabled, port pins are dedicated to SPI functions */ 
    }
    
}
/*
************************************************************************************************
*  Function:         EE_Init()
*  Description:      EEPROM初始化函数
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           设置EEPROM时钟频率(Set Fflash(1MHz ≈ 8MHz / 0b010111 > 0.8MHz), needed: Fbus = 24MHz)              
************************************************************************************************
*/
void EE_Init(void)
{
    while (!FSTAT_CCIF);					                       /* Wait for FTM reset to complete */ 
    
    FCLKDIV     = 0b00000111;               
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ | 
	                  |||||_____ |bit5:0,set to divide BUSCLK down to 1MHz,Fbus = 24MHz,
	                  ||||______ |       FDIV[6:0] is recommanded as 0x17(1MHz ≈ 24MHz / 0b010111 > 0.8MHz);
	                  |||_______/                                                      
	                  ||________  bit6, FDIV is open for writing;;
	                  |_________  bit7, FCLKDIV register has  been written since the last reset or not(read only); 
    */
}