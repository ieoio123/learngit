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
*Descriprion :   ����Fbus = 24Hz��S9S12G128�ĵ�Ƭ��ƽ̨
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
static INT16U sBLCH_PT01AD;       /* ��������ʱ��IO��PT01AD�ľ���ͨ�� */
static INT8U  sBLCH_PTT;          /* ��������ʱ��IO��PTT�ľ���ͨ�� */
static INT8U  sBLCH_PTP;          /* ��������ʱ��IO��PTP�ľ���ͨ�� */
static INT16U sBLCH_PT01AD_CLR;   /* �رվ���ʱ��IO��PT01AD�ľ���ͨ�� */
static INT8U  sBLCH_PTT_CLR;      /* �رվ���ʱ��IO��PTT�ľ���ͨ�� */
static INT8U  sBLCH_PTP_CLR;      /* �رվ���ʱ��IO��PTP�ľ���ͨ�� */

void IoPwm_Init(void)
{
    /* ��¼ģ��PWMǰ��IO��ʼֵ */
    sBLCH_PT01AD_CLR = PT01AD;    
    sBLCH_PTT_CLR    = PTT;
    sBLCH_PTP_CLR    = PTP; 
}
/*
************************************************************************************************
*  Function:         IO_PWM_Set()
*  Description:      ģ��PWM��IO�ھ���ͨ������
*  Calls:            IO_PWM_UPDOWN();
*  Input:            INT16U blch_n,INT16U blch_p,INT8U mode
*  Output:           �� 
*  Return:           BOOLEAN
*  Others:           �������:blch_nΪNMOS����ͨ��,blch_nΪPMOS����ͨ��,
*                    modeΪ����ģʽ:0,���ھ���ģʽ;1,������ģʽ; 
*                    ���ؾ���ͨ���Ŀ���״̬       
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
    if(blch_n & blch_p)                               /* ����ͨ������ */
    {
        while(!CPMUFLG_RTIF);                         /* ϵͳ����500ms */
        return FALSE;
    }else if((blch_n == 0) && (blch_p == 0))          /* ������� */
    {
        while(!CPMUFLG_RTIF);                         /* ϵͳ����500ms */
        return TRUE;
    }
    /* ģ��PWM��IO�ڳ�ʼ�� */ 
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
    /* ��ȡģ��PWM��IO�ڵĿ���״̬ */
    sBLCH_PT01AD = (INT16U)(io_blch >> 16);
    sBLCH_PTT    = (INT8U)(io_blch >> 8);
    sBLCH_PTP    = (INT8U)(io_blch);
    
    if((mode == 1) || (mode == 255))
    {
        while(!CPMUFLG_RTIF)   /* �ȴ�����500ms��� */
        {
            /* ��������ͨ�� */
            PT01AD = sBLCH_PT01AD;
            PTT   = sBLCH_PTT;
            PTP   = sBLCH_PTP;
            for(tick = 0; tick < TICK1_CONDUCTION; tick++); 
            for(tick = 0; tick < TICK1_CONDUCTION; tick++); 
            for(tick = 0; tick < TICK1_CONDUCTION; tick++); 
            /* �رվ���ͨ�� */
            PT01AD  = sBLCH_PT01AD_CLR;
            PTT    = sBLCH_PTT_CLR;
            PTP    = sBLCH_PTP_CLR;   
            for(tick = 0; tick < TICK1_SHUTDOWN; tick++);
            for(tick = 0; tick < TICK1_SHUTDOWN; tick++);
            for(tick = 0; tick < TICK1_SHUTDOWN; tick++);
        }
    }else if(mode == 2)
    {
        while(!CPMUFLG_RTIF)   /* �ȴ�����500ms��� */
        {
            /* ��������ͨ�� */
            PT01AD = sBLCH_PT01AD;
            PTT   = sBLCH_PTT;
            PTP   = sBLCH_PTP;    
            for(tick = 0; tick < TICK2_CONDUCTION; tick++); 
            for(tick = 0; tick < TICK2_CONDUCTION; tick++);
            for(tick = 0; tick < TICK2_CONDUCTION; tick++);
            /* �رվ���ͨ�� */
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