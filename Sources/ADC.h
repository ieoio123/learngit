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
#ifndef __ADC_H
#define __ADC_H

#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
#define FILTER_NUM 10       /* �����¶������˲���������С,range:1~12 */

#if((FILTER_NUM < 1)||(FILTER_NUM > 12))    /* �˲����������ò���ȷ */
    #error
#endif
/*********************************Function declaration****************************************/
void ADC_Init(void);
INT16U AdcConvert(INT8U ch);
void AdcTemp(const INT8U *p_ch,INT8U *p_temp);
INT8U AdcFilter(INT8U *p_value);

#endif