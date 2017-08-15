/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     DataLoad.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __DATALOAD_H
#define __DATALOAD_H

#include ""
#define NUMFRAME         5                         /* CAN数据帧总数             */
#define IDCELLVOL_N1     Id_Array[NUMCUR][0]       /* 单电池电压消息1           */
#define IDCELLVOL_N2     Id_Array[NUMCUR][1]       /* 单电池电压消息2           */
#define IDCELLVOL_N3     Id_Array[NUMCUR][2]       /* 单电池电压消息3           */
#define IDCELLVOL_N4     Id_Array[NUMCUR][3]       /* 单电池电压消息4           */
#define IDTEMP           Id_Array[NUMCUR][4]       /* 温度消息                  */ 
#define IDMSG1           Id_Array[NUMCUR][5]       /* 电池模块消息1             */ 
#define IDMSG2           Id_Array[NUMCUR][6]       /* 电池模块消息2             */ 
#define IDVER            Id_ArrayVer[NUMCUR]       /* 电池模块版本信息          */
#define PRTY_TEMP        4


#endif