/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     IO_PWM.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   适用Fbus = 24Hz的S9S12G128的单片机平台
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-16  Zeng Qunxin
***********************************************************************************************/
#include "INCLUDES.h"
/************************************const value define***************************************/
const INT32U IoPwm_ChN[IOPWMCH_SIZE] = 
{
    IOPWM_N1, IOPWM_N2, IOPWM_N3, IOPWM_N4, IOPWM_N5, IOPWM_N6, 
    IOPWM_N7, IOPWM_N8, IOPWM_N9, IOPWM_N10, IOPWM_N11
};
const INT32U IoPwm_ChP[IOPWMCH_SIZE] = 
{
    IOPWM_P1, IOPWM_P2, IOPWM_P3, IOPWM_P4, IOPWM_P5, IOPWM_P6, 
    IOPWM_P7, IOPWM_P8, IOPWM_P9, IOPWM_P10, IOPWM_P11   
};
/************************************Variable define******************************************/
static INT16U sBLCH_PT01AD;       /* 开启均衡时的IO口PT01AD的均衡通道 */
static INT8U  sBLCH_PTT;          /* 开启均衡时的IO口PTT的均衡通道 */
static INT8U  sBLCH_PTP;          /* 开启均衡时的IO口PTP的均衡通道 */
static INT16U sBLCH_PT01AD_CLR;   /* 关闭均衡时的IO口PT01AD的均衡通道 */
static INT8U  sBLCH_PTT_CLR;      /* 关闭均衡时的IO口PTT的均衡通道 */
static INT8U  sBLCH_PTP_CLR;      /* 关闭均衡时的IO口PTP的均衡通道 */

void IoPwm_Init(void)
{
    /* 记录模拟PWM前的IO初始值 */
    sBLCH_PT01AD_CLR = PT01AD;    
    sBLCH_PTT_CLR    = PTT;
    sBLCH_PTP_CLR    = PTP; 
}
/*
************************************************************************************************
*  Function:         IO_PWM_Set()
*  Description:      模拟PWM的IO口均衡通道设置
*  Calls:            IO_PWM_UPDOWN();
*  Input:            INT16U blch_n,INT16U blch_p,INT8U mode
*  Output:           无 
*  Return:           BOOLEAN
*  Others:           输入参数:blch_n为NMOS均衡通道,blch_n为PMOS均衡通道,
*                    mode为均衡模式:0,组内均衡模式;1,组间均衡模式; 
*                    返回均衡通道的开启状态       
************************************************************************************************
*/
BOOLEAN IoPwm_Set(INT32U blch,INT8U mode)
{
    INT32U io_blch = 0;
    INT8U i = 0;
    INT16U blch_n = ((INT16U)(blch >> 16) & ACTIVE_BLMASK);
    INT16U blch_p = ((INT16U)(blch) & ACTIVE_BLMASK);
    INT8U tick = 0;
    
    if(mode == 255)
    {
        Rtc20ms();    
    }else
    {
        Rtc500ms();
    }
    if(blch_n & blch_p)                               /* 均衡通道错误 */
    {
        while(!CPMUFLG_RTIF);                         /* 系统空闲500ms */
        return FALSE;
    }else if((blch_n == 0) && (blch_p == 0))          /* 无需均衡 */
    {
        while(!CPMUFLG_RTIF);                         /* 系统空闲500ms */
        return TRUE;
    }
    /* 模拟PWM的IO口初始化 */ 
    io_blch = sBLCH_PT01AD_CLR;
    io_blch = (io_blch << 16) | sBLCH_PTT_CLR;
    io_blch = (io_blch << 8) | sBLCH_PTP_CLR;  
    for(i = 0; i < IOPWMCH_SIZE; i++)
    {
        if(blch_n & ((INT16U)0x0001 << i))
        {
            io_blch |= IoPwm_ChN[i];    
        }
        if(blch_p & ((INT16U)0x0001 << i))
        {
            io_blch &= IoPwm_ChP[i];    
        }
    }
    /* 获取模拟PWM的IO口的开启状态 */
    sBLCH_PT01AD = (INT16U)(io_blch >> 16);
    sBLCH_PTT    = (INT8U)(io_blch >> 8);
    sBLCH_PTP    = (INT8U)(io_blch);
    
    if((mode == 1) || (mode == 255))
    {
        while(!CPMUFLG_RTIF)   /* 等待均衡500ms完成 */
        {
            /* 开启均衡通道 */
            PT01AD = sBLCH_PT01AD;
            PTT   = sBLCH_PTT;
            PTP   = sBLCH_PTP;
            for(tick = 0; tick < TICK1_CONDUCTION; tick++); 
            for(tick = 0; tick < TICK1_CONDUCTION; tick++); 
            for(tick = 0; tick < TICK1_CONDUCTION; tick++); 
            /* 关闭均衡通道 */
            PT01AD  = sBLCH_PT01AD_CLR;
            PTT    = sBLCH_PTT_CLR;
            PTP    = sBLCH_PTP_CLR;   
            for(tick = 0; tick < TICK1_SHUTDOWN; tick++);
            for(tick = 0; tick < TICK1_SHUTDOWN; tick++);
            for(tick = 0; tick < TICK1_SHUTDOWN; tick++);
        }
    }else if(mode == 2)
    {
        while(!CPMUFLG_RTIF)   /* 等待均衡500ms完成 */
        {
            /* 开启均衡通道 */
            PT01AD = sBLCH_PT01AD;
            PTT   = sBLCH_PTT;
            PTP   = sBLCH_PTP;    
            for(tick = 0; tick < TICK2_CONDUCTION; tick++); 
            for(tick = 0; tick < TICK2_CONDUCTION; tick++);
            for(tick = 0; tick < TICK2_CONDUCTION; tick++);
            /* 关闭均衡通道 */
            PT01AD  = sBLCH_PT01AD_CLR;
            PTT    = sBLCH_PTT_CLR;
            PTP    = sBLCH_PTP_CLR; 
            for(tick = 0; tick < TICK2_SHUTDOWN; tick++);
            for(tick = 0; tick < TICK2_SHUTDOWN; tick++);
            for(tick = 0; tick < TICK2_SHUTDOWN; tick++); 
        }
    }else
    {
        while(!CPMUFLG_RTIF);
        return FALSE;
    }
    return TRUE;
}