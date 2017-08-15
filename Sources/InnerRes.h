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
#define INNER_RES_INIT_VALUE   10     /* 定义整个电池包欧姆内阻初始值为0.5欧姆 */
#define MATRIX 100000                 /* 收敛矩阵的初始值 */
#define THITA0 900                    /* 参数 分辨率0.001 */
#define THITA1 500                    /* 欧姆内阻初始值0.5欧姆 */
#define THITA2 100                    /* 参数 分辨率0.001 */
/*********************************Function declaration****************************************/
void InnerRes_Init(void);
void InnerResEstimation(INT16S current) ;

#endif