/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     SPI.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __SPI_H
#define __SPI_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
/* SPI1通信函数 */ 
#define SPI1_COMM(wrdata,rddata)   \
        {while(!SPI1SR_SPTEF);rddata=SPI1SR;SPI1DRL = wrdata;while(!SPI1SR_SPIF);rddata=SPI1SR;rddata = SPI1DRL;}
        
#define SPI0_SS PTJ_PTJ7     /* SPI0使能管脚 */
#define SPI1_SS PTJ_PTJ3     /* SPI1使能管脚 */
/*********************************Function declaration****************************************/        
void SPI1_Init(void);

#endif