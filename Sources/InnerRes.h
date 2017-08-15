/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     InnerRes.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2012-03-19   Zeng Qunxin
**********************************************************************************************/
#ifndef __INNER_RES_H_
#define __INNER_RES_H_     

#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
#define INNER_RES_INIT_VALUE   10     /* ����������ذ�ŷķ�����ʼֵΪ0.5ŷķ */
#define MATRIX 100000                 /* ��������ĳ�ʼֵ */
#define THITA0 900                    /* ���� �ֱ���0.001 */
#define THITA1 500                    /* ŷķ�����ʼֵ0.5ŷķ */
#define THITA2 100                    /* ���� �ֱ���0.001 */
/*********************************Function declaration****************************************/
void InnerRes_Init(void);
void InnerResEstimation(INT16S current) ;

#endif