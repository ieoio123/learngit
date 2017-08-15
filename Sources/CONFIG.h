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

#define NUMCUR           0//EE_WordRead(0x0708)  /* LECU������              */
#define NUMMODULE        1//24                   /* ģ����                  */
/*******************************Management number of cell Setting*****************************/  
          
#define NUMCELL          15                   /* LECU�����ص�����        */       
#define PASSIVE_BLMASK   0x3FFF               /* ������������              */
#define BLTHRESHOLD_MIN  2000                 /* ��С������ֵ��ѹ*/
/************************************Temp Message Setting*************************************/

#define NUMTEMP          5//3                    /* ����TEMP��������Ŀ        */  

#define ADCCH0           2                    /* TEMP1   ��Ӧ��ADCͨ��ANx  */
#define ADCCH1           9                    /* TEMP2   ��Ӧ��ADCͨ��ANx  */
#define ADCCH2           1                    /* TEMP3   ��Ӧ��ADCͨ��ANx  */
#define ADCCH3           8                    /* TEMP2   ��Ӧ��ADCͨ��ANx  */
#define ADCCH4           0                    /* TEMP3   ��Ӧ��ADCͨ��ANx  */

#define ADCCH_PCB        10                    /* �����¶ȶ�Ӧ��ADCͨ��ANx  */
/************************************LED Location Setting*************************************/
#define LED      DDRJ_DDRJ5                   /* �ƿ��ƶ˿ڵķ���Ĵ���    */
#define LEDDATA  PTJ_PTJ5                     /* �ƿ��ƶ˿ڵ����ݼĴ���    */
/************************************FAN Location Setting*************************************/
#define FANOUT   DDRJ_DDRJ4                   /* ���ȿ��ƶ˿ڵķ���Ĵ���  */
#define FANDATA  PTJ_PTJ4                     /* ���ȿ��ƶ˿ڵ����ݼĴ���  */
/************************************CAN Message Setting**************************************/
#define IDCANCMD         0x011                /* LECU CAN����ͨѶʹ��ID    */
#define IDCANDIAG        0x051                /* LECU CAN���ղ���ID        */
#define IDCANQUERY       0x400                /* LECU CAN�汾��ѯID        */
#define DIAG_CODE        0x52C0
#define CCP_CODE         0xFFFF

#endif




