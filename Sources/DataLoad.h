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
#define NUMFRAME         5                         /* CAN����֡����             */
#define IDCELLVOL_N1     Id_Array[NUMCUR][0]       /* ����ص�ѹ��Ϣ1           */
#define IDCELLVOL_N2     Id_Array[NUMCUR][1]       /* ����ص�ѹ��Ϣ2           */
#define IDCELLVOL_N3     Id_Array[NUMCUR][2]       /* ����ص�ѹ��Ϣ3           */
#define IDCELLVOL_N4     Id_Array[NUMCUR][3]       /* ����ص�ѹ��Ϣ4           */
#define IDTEMP           Id_Array[NUMCUR][4]       /* �¶���Ϣ                  */ 
#define IDMSG1           Id_Array[NUMCUR][5]       /* ���ģ����Ϣ1             */ 
#define IDMSG2           Id_Array[NUMCUR][6]       /* ���ģ����Ϣ2             */ 
#define IDVER            Id_ArrayVer[NUMCUR]       /* ���ģ��汾��Ϣ          */
#define PRTY_TEMP        4


#endif