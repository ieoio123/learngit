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
#ifndef __MCU_H
#define __MCU_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
#define RtcOn()  {CPMUINT_RTIE = 1;}          /* Enable Real Time Interrupt */
#define RtcOff() {CPMUINT_RTIE = 0;}          /* Disable Real Time Interrupt */
#define DELAY_1MS 3460                        /* ms延时计数值 */
/*********************************Function declaration****************************************/
void MCG_Init(void);
void MCG_DelayMs(INT16U time_ms);

#endif

