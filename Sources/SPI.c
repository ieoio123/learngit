/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     SPI.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   适用于MC9S12G128单片机平台
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "SPI.h"
#include "TYPEDEFINE.h"
/*
************************************************************************************************
*  Function:         SPI1_Init()
*  Description:      SPI1模块初始化函数
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           SPI1模块时钟管脚和工作模式初始化(set Fspi = 1MHz, needed: Fbus = 8MHz)                
************************************************************************************************
*/
void SPI1_Init(void)
{
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
    SPI1BR      = 0b00000010; 
    /*              ||||||||_  
	                  |||||||__\  
	                  ||||||___/ bit2:0, SPR  = 0b010;
	                  |||||____  
	                  ||||_____\  
	                  |||______/ bit6:4, SPPR = 0b000;Fspi = Fbus/((0b000 + 1) * 2^(0b010 + 1)) = 1MHz;  
	                  ||_______  
	                  |________  
    */
    SPI1CR1_SPE = 1;         /* SPI enabled, port pins are dedicated to SPI functions */ 
    DDRJ_DDRJ3 = 1;
    PTJ_PTJ3 = 1;
}
