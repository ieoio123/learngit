/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     Ltc6803.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __LTC6803_H
#define __LTC6803_H

#include "TYPEDEFINE.h"
#include "MixDriver.h"
#include "CONFIG.h"
/***************************************Macro define******************************************/                      
#define CELLNUM NUMCELL          /* 模块管理电池节数 */
#if(CELLNUM == 15) 
    #define CELLNUM_BOTTOM 7
    #define NUMMASK_BOTTOM 0x7F
    #define CELLNUM_TOP 8
    #define NUMMASK_TOP 0xFF   //0x7F   by han
#else
    #error
#endif
                            
#define SPI_ON 0            /* SPI通讯连接 */
#define SPI_OFF 1           /* SPI通讯断开 */
#define MASK_CELL_IR 0x000  /* Mask Cell x Interrupts */    
#define OPEN_VOL_DIFF 1000  /* Open-Wire 阈值 */

#define WRCFG   0x01 /* Write Configuration Register Group                                                       */
#define RDCFG   0x02 /* Read Configuration Register Group                                                        */
#define RDCV    0x04 /* Read Cell Voltage Register Group                                                         */
#define RDFLG   0x06 /* Read Flag Register Group                                                                 */
#define RDTMP   0x0E /* Read Temperature Register Group                                                          */
#define STCVAD  0x10 /* Start Cell Voltage A/D Conversions and Poll Status (all cell)                            */
#define STOWAD  0x20 /* Start Open Wire A/D Conversions and Poll Status (all cell)                               */
#define STTMPAD 0x33 /* Start Temperature A/D Conversions and Poll Status(Internal temperature)                  */
#define PLADC   0x40 /* Poll A/D Converter Status                                                                */
#define PLINT   0x50 /* Poll Interrupt Status                                                                    */
#define STCVDC  0x60 /* Start Cell Voltage A/D Conversions and Poll Status, with Discharge Permitted (all cell)  */
#define STOWDC  0x70 /* Start Open Wire A/D Conversions and Poll Status, with Discharge Permitted (all cell)     */
#define DAGN    0x52 /* Start Diagnose and Poll Status */
#define RDDGNR  0x54 /* Read Diagnostic Register */

#define LTC6803_COMM(wrdata, rddata)  SPI1_COMM(wrdata, rddata)      /* LTC6803的SPI通讯 */
/*********************************Function declaration****************************************/
BOOLEAN Ltc6803Devstatus(void);
BOOLEAN Ltc6803CfgInit(INT32U blch);
BOOLEAN Ltc6803CellVolDetect(INT16U *p_vcell);
BOOLEAN Ltc6803OpenDetect(void);
void Ltc6803TempDectect(INT8U *p_temp);
BOOLEAN Ltc6803Cfg(INT32U blchannel) ; 
void Ltc6803Wr(INT8U cmd);
void Ltc6803Rd(INT16U *cell);
INT8U Ltc6803RdTemp(void);
INT8U Crc107(INT8U *buff, INT8U len);
BOOLEAN Ltc6803Diag(void);
#endif
