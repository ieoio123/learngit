/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     IO_PWM.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-16  Zeng Qunxin
***********************************************************************************************/
#ifndef __IOPWM_H
#define __IOPWM_H

#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
#if(0)
#define TICK1_CONDUCTION 73     //73//N:0.465-0.315=0.15A   11KHz (3.34V,3.4V)
#define TICK1_SHUTDOWN   66     //60//P:0.5-0.34=0.16A    (3.34V,3.4V)
#define TICK2_CONDUCTION 95     //95//N:0.605-0.31=0.295A  9.55KHz (3.34V,3.4V)
#define TICK2_SHUTDOWN   66     //66//P:0.64-0.38=0.26A    (3.34V,3.4V)
#endif
#define TICK1_CONDUCTION 42     //73//N:0.465-0.315=0.15A   11KHz (3.34V,3.4V)
#define TICK1_SHUTDOWN   40     //60//P:0.5-0.34=0.16A    (3.34V,3.4V)
#define TICK2_CONDUCTION 58     //95//N:0.605-0.31=0.295A  9.55KHz (3.34V,3.4V)
#define TICK2_SHUTDOWN   50     //50//66//P:0.64-0.38=0.26A    (3.34V,3.4V)
#define IOPWMCH_SIZE 11
#define IOPWM_N1    (INT32U)0x2000000  /* PT0AD_PT0AD1 */
#define IOPWM_N2    (INT32U)0x0001000  /* PTT_PTT4     */
#define IOPWM_N3    (INT32U)0x0000800  /* PTT_PTT3     */
#define IOPWM_N4    (INT32U)0x0000400  /* PTT_PTT2     */
#define IOPWM_N5    (INT32U)0x0000200  /* PTT_PTT1     */
#define IOPWM_N6    (INT32U)0x0010000  /* PT1AD_PT1AD0 */
#define IOPWM_N7    (INT32U)0x0000100  /* PTT_PTT0     */
#define IOPWM_N8    (INT32U)0x1000000  /* PT0AD_PT0AD0 */
#define IOPWM_N9    (INT32U)0x0020000  /* PT1AD_PT1AD1 */
#define IOPWM_N10   (INT32U)0x0040000  /* PT1AD_PT1AD2 */
#define IOPWM_N11   (INT32U)0x4000000  /* PT0AD_PT0AD2 */
#define IOPWM_P1  (~(INT32U)0x0000002) /* PTP_PTP1     */
#define IOPWM_P2  (~(INT32U)0x0000004) /* PTP_PTP2     */
#define IOPWM_P3  (~(INT32U)0x0000008) /* PTP_PTP3     */
#define IOPWM_P4  (~(INT32U)0x0000010) /* PTP_PTP4     */
#define IOPWM_P5  (~(INT32U)0x0000040) /* PTP_PTP6     */
#define IOPWM_P6  (~(INT32U)0x0000020) /* PTP_PTP5     */
#define IOPWM_P7  (~(INT32U)0x0000080) /* PTP_PTP7     */
#define IOPWM_P8  (~(INT32U)0x0008000) /* PTT_PTT7     */
#define IOPWM_P9  (~(INT32U)0x0000001) /* PTP_PTP0     */
#define IOPWM_P10 (~(INT32U)0x0004000) /* PTT_PTT6     */
#define IOPWM_P11 (~(INT32U)0x0002000) /* PTT_PTT5     */
/*********************************Function declaration****************************************/
void IoPwm_Init(void);
BOOLEAN IoPwm_Set(INT32U blch,INT8U mode);
void IO_PWM_UPDOWN(INT8U mode);

#endif
