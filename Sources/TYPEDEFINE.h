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
/* CAN接收信息结构体 */
typedef struct canrxmsg
{
    INT16U id;                     /* 11位ID为标准格式标识符         */
    INT8U  rtr;                    /* 远程发送请求,0 数据帧 1 远程帧 */
    INT8U  data[8];                /* 数据                           */
    INT8U  len;                    /* 数据长度                       */
    BOOLEAN  newdata;              /* 新数据                         */
}CANRX_MSG;
/* CAN发送信息结构体 */
typedef struct cantxmsg
{
    INT16U id;                     /* 11位ID为标准格式标识符         */
    INT8U  rtr;                    /* 远程发送请求,0 数据帧 1 远程帧 */
    INT8U  data[8];                /* 数据                           */
    INT8U  len;                    /* 数据长度                       */
    INT8U  prty;                   /* 优先级                         */
    struct cantxmsg *next;                /* 发送信息指针                   */
} CANTX_MSG;

/***************************************Macro define******************************************/
#define TRUE 1
#define FALSE 0

#endif