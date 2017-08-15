/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     MCU.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __MIXDRIVER_H
#define __MIXDRIVER_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
#define RtcOn()  {CPMUINT_RTIE = 1;}          /* Enable Real Time Interrupt */
#define RtcOff() {CPMUINT_RTIE = 0;}          /* Disable Real Time Interrupt */
#define TICK_DELAY_1MS 3460                        /* ms延时计数值 */
/* SPI1通信函数 */ 
#define SPI1_COMM(wrdata,rddata)   \
        {while(!SPI1SR_SPTEF);rddata=SPI1SR;SPI1DRL = wrdata;while(!SPI1SR_SPIF);rddata=SPI1SR;rddata = SPI1DRL;}
/* 风扇控制脚 */    
#define FANOUT   DDRJ_DDRJ4               /* 风扇控制端口的方向寄存器 */
#define FANDATA  PTJ_PTJ4                     /* 风扇控制端口的数据寄存器 */
/* LED灯控制脚 */ 
#define LED      DDRJ_DDRJ5                   /* 灯控制端口的方向寄存器   */
#define LEDDATA  PTJ_PTJ5                     /* 灯控制端口的数据寄存器   */
/* Ltc6803-1使能脚 */
#define LTC6803_OUT  DDRJ_DDRJ3
#define LTC6803_SS   PTJ_PTJ3 
/* 供电控制脚 */    
#define POWERCTRLOUT1   DDRJ_DDRJ6               /* 供电控制端口的方向寄存器 */
#define POWERCTRLDATA1 PTJ_PTJ6                 /* 供电控制端口的数据寄存器 */
#define POWERCTRLOUT2   DDRJ_DDRJ7               /* 供电控制端口的方向寄存器 */
#define POWERCTRLDATA2 PTJ_PTJ7                 /* 供电控制端口的数据寄存器 */

/*********************************Function declaration****************************************/
void MCG_Init(void);
void Rtc500ms(void);
void MCG_DelayMs(INT16U time_ms);
void IO_Init(void);
void FAN_Ctrl(BOOLEAN status);
void ADC_Init(void);
INT16U ADC_Convert(INT8U ch);
void SPI1_Init(void);
void EE_Init(void);

#endif

