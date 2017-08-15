/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     CONIFG.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#include "TYPEDEFINE.h"
#include "Flash.h"

#define NUMCUR           0//EE_WordRead(0x0708)  /* LECU软件编号              */
#define NUMMODULE        1//24                   /* 模块编号                  */
/*******************************Management number of cell Setting*****************************/  
          
#define NUMCELL          15                   /* LECU管理电池单体数        */       
#define PASSIVE_BLMASK   0x3FFF               /* 主动均衡掩码              */
#define BLTHRESHOLD_MIN  2000                 /* 最小均衡阈值电压*/
/************************************Temp Message Setting*************************************/

#define NUMTEMP          5//3                    /* 板外TEMP采样线数目        */  

#define ADCCH0           2                    /* TEMP1   对应的ADC通道ANx  */
#define ADCCH1           9                    /* TEMP2   对应的ADC通道ANx  */
#define ADCCH2           1                    /* TEMP3   对应的ADC通道ANx  */
#define ADCCH3           8                    /* TEMP2   对应的ADC通道ANx  */
#define ADCCH4           0                    /* TEMP3   对应的ADC通道ANx  */

#define ADCCH_PCB        10                    /* 板上温度对应的ADC通道ANx  */
/************************************LED Location Setting*************************************/
#define LED      DDRJ_DDRJ5                   /* 灯控制端口的方向寄存器    */
#define LEDDATA  PTJ_PTJ5                     /* 灯控制端口的数据寄存器    */
/************************************FAN Location Setting*************************************/
#define FANOUT   DDRJ_DDRJ4                   /* 风扇控制端口的方向寄存器  */
#define FANDATA  PTJ_PTJ4                     /* 风扇控制端口的数据寄存器  */
/************************************CAN Message Setting**************************************/
#define IDCANCMD         0x011                /* LECU CAN接收通讯使能ID    */
#define IDCANDIAG        0x051                /* LECU CAN接收测试ID        */
#define IDCANQUERY       0x400                /* LECU CAN版本查询ID        */
#define DIAG_CODE        0x52C0
#define CCP_CODE         0xFFFF

#endif




