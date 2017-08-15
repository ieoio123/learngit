/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     Diagnostic.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2012-03-19   Zeng Qunxin
**********************************************************************************************/
#ifndef __DIAGNOSTIC_H
#define __DIAGNOSTIC_H

#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/

/****************************************Error Code*******************************************/

#define CELL_TEMPAD_HI_II          (INT16U)(0x0100)         /* 单体温度传感器电压过高, 10位AD值     */
#define CELL_TEMPAD_LO_II          (INT16U)(0x0200)         /* 单体温度传感器电压过低, 10位AD值     */

#define PCB_TEMP_HI_I              (INT16U)(0x0400)         /* 130℃, LECU 板上温度极高             */
#define PCB_TEMP_HI_II             (INT16U)(0x0800)         /* 110℃, LECU 板上温度过高             */
#define PCB_TEMP_HI_III            (INT16U)(0x1000)         /* 90℃, LECU 板上温度微高              */

#define PCB_TEMPAD_HI_II           (INT16U)(0x2000)         /* LECU板上温度传感器电压过高, 10位AD值 */
#define PCB_TEMPAD_LO_II           (INT16U)(0x4000)         /* LECU板上温度传感器电压过高, 10位AD值 */

#define LECU_OPEN_CIRCUIT          (INT16U)(0x8000)         /* LECU 断路                            */
#define LECU_SPI_ERROR             (INT16U)(0x0001)         /* LECU SPI 故障                        */
#define LECU_FAN_ERROR             (INT16U)(0x0002)         /* LECU 风扇故障                        */
#define LECU_BALANCE_ERROR         (INT16U)(0x0004)         /* LECU 均衡功能故障                    */
#define LECU_SAMPLE_ERROR          (INT16U)(0x0004)         /* LECU 采样故障                        */
/********************************Tempature Threshold Setting***********************************/

#define CELL_TEMPAD_HI_II_TH          1022        /* 单体温度传感器电压过高, 10位AD值     */
#define CELL_TEMPAD_LO_II_TH          100         /* 单体温度传感器电压过低, 10位AD值     */

#define PCB_TEMP_HI_I_TH              130         /* 90℃, LECU 板上温度极高              */
#define PCB_TEMP_HI_II_TH             120         /* 80℃, LECU 板上温度过高              */
#define PCB_TEMP_HI_III_TH            110         /* 70℃, LECU 板上温度微高              */

#define PCB_TEMPAD_HI_II_TH           1022        /* LECU板上温度传感器电压过高, 10位AD值 */
#define PCB_TEMPAD_LO_II_TH           100         /* LECU板上温度传感器电压过高, 10位AD值 */

/*********************************Function declaration****************************************/
void Diag_Process(void);
void Diag_TempAD(INT16U *p_advalue, INT8U num);
void Diag_PcbTempAD(INT16U advalue);
void Diag_PcbTemp(INT8U pcb_temp);
void Diag_Open_Circuit(void);
void Diag_Spi(void);
void Diag_Balance(void);
void Diag_Ltc6803(void);

#endif