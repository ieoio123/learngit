/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     TYPEDEFINE.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __TYPEDEFINE_H
#define __TYPEDEFINE_H

/***************************************Type define*******************************************/
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */
/* CAN������Ϣ�ṹ�� */
typedef struct canrxmsg
{
    INT16U id;                     /* 11λIDΪ��׼��ʽ��ʶ��         */
    INT8U  rtr;                    /* Զ�̷�������,0 ����֡ 1 Զ��֡ */
    INT8U  data[8];                /* ����                           */
    INT8U  len;                    /* ���ݳ���                       */
    BOOLEAN  newdata;              /* ������                         */
}CANRX_MSG;
/* CAN������Ϣ�ṹ�� */
typedef struct cantxmsg
{
    INT16U id;                     /* 11λIDΪ��׼��ʽ��ʶ��         */
    INT8U  rtr;                    /* Զ�̷�������,0 ����֡ 1 Զ��֡ */
    INT8U  data[8];                /* ����                           */
    INT8U  len;                    /* ���ݳ���                       */
    INT8U  prty;                   /* ���ȼ�                         */
    struct cantxmsg *next;                /* ������Ϣָ��                   */
} CANTX_MSG;

/***************************************Macro define******************************************/
#define TRUE 1
#define FALSE 0

#endif