/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     ADC.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __TEMPCOLLPROCESS_H
#define __TEMPCOLLPROCESS_H

#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
#define NUMTEMP     3

#define RESTEMP_SIZE    161  /* RESTEMP温度范围为-40 <= TEMP <= 120 */
#define PCBRESTEMP_SIZE 191  /* RESTEMP温度范围为-40 <= TEMP <= 150 */   
#define FILTER_NUM 10        /* 定义温度数据滤波缓冲区大小,range:1~12 */

//#define ADCCH0           7                    /* TEMP1   对应的ADC通道ANx  */
//#define ADCCH1           6                    /* TEMP2   对应的ADC通道ANx  */
//#define ADCCH2           4                    /* TEMP3   对应的ADC通道ANx  */
//#define ADCCH_PCB        5                    /* 板上温度对应的ADC通道ANx  */

#define SERIE_SRES       1000                 /* 串联电阻为1000 * 10欧姆   */
#define PCB_SERIE_SRES   100                  /* 串联电阻为100  * 10欧姆   */
/*********************************Function declaration****************************************/
INT8U ResTemp_LookUp(INT8U value, INT8U *p_tab,INT8U size);
void  Temp_Samp(INT8U *p_temp);
INT8U ResTemp_Filter(INT8U *p_value);

#endif