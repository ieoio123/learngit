/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     SampAndProcess.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __SAMPANDPROCESS_H
#define __SAMPANDPROCESS_H

#include "TYPEDEFINE.h"
/***************************************Type define*******************************************/
typedef struct lecu_info
{
    INT16U CellVol[NUMCELL];
    INT16U ModuleVol;
    INT16U MaxCellVol;
    INT16U MinCellVol;
    INT8U  MaxCellVolIndex;
    INT8U  MinCellVolIndex;
    
    INT8U  MaxTemp;
    INT8U  MinTemp;
    INT8U  MaxTempIndex;
    INT8U  MinTempIndex;
    INT8U  Temp[NUMTEMP];
    INT8U  PcbTemp;
    
    INT8U  InnerRes[NUMCELL];
    INT8U  MaxInnerRes;
    INT8U  MinInnerRes;
    INT8U  MaxInnerResIndex;
    INT8U  MinInnerResIndex;
    
    INT16U ErrCode;
    INT8U  RunMode;
    INT8U  WorkMode;
    INT32U BlCh;
    BOOLEAN FanCtrl;
    INT32U BlTimeSum;
    
} LECU_INFO;  
/***************************************Macro define******************************************/

#define RESTEMP_SIZE    251  /* RESTEMP温度范围为-40 <= TEMP <= 85 */
#define PCBRESTEMP_SIZE 166  /* RESTEMP温度范围为-40 <= TEMP <= 150 */ 
#define EF1I103KT10T02009G011
#define CN0603R103B3435FB

#define FILTER_NUM 10        /* 定义温度数据滤波缓冲区大小,range:1~12 */

#define SERIE_SRES       1000                 /* 串联电阻为1000 * 10欧姆   */
#define PCB_SERIE_SRES   100                  /* 串联电阻为100  * 10欧姆   */

/**************************************WorkMode 的值******************************************/
#define IDLE 0
#define WORK 1
#define DIAG 2
/*********************************Function declaration****************************************/
INT8U ResTemp_LookUp(INT16U value, INT16U *p_tab,INT8U size);
void  ResTemp_Samp(void);
INT8U ResTemp_Filter(INT8U *p_value);
void Vol_Samp(void);
void MaxMinVol_LookUp(void);
void MaxMinTemp_LookUp(void);
void MaxMinInnerRes_LookUp(void);

#endif