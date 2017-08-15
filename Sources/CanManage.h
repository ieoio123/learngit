#ifndef __CANMANAGE_H_
#define __CANMANAGE_H_  

#include "INCLUDES.h"
/***************************************Type define*******************************************/
typedef struct LECURX_MSG
{
    INT32U  EquTime;        /* 单位为0.5s */
    INT8U   EquCmd;
    INT16U  EquVol;
    INT16S  Cur;
    BOOLEAN FanCtrl;
    BOOLEAN CommTimeOut;
    
    BOOLEAN DiagFlg;
    INT16U  DiagEquCh;
    INT8U   DiagEquDir;
    BOOLEAN DiagFanCtrl;
    
    BOOLEAN VerFlg;
} LECURX_MSG;
/***************************************Macro define******************************************/
#define IDCELLVOL_N1     Id_Array[NUMCUR][0]       /* 单电池电压消息1           */
#define IDCELLVOL_N2     Id_Array[NUMCUR][1]       /* 单电池电压消息2           */
#define IDCELLVOL_N3     Id_Array[NUMCUR][2]       /* 单电池电压消息3           */
#define IDCELLVOL_N4     Id_Array[NUMCUR][3]       /* 单电池电压消息4           */
#define IDTEMP           Id_Array[NUMCUR][4]       /* 温度消息                  */ 
#define IDMSG1           Id_Array[NUMCUR][5]       /* 电池模块消息1             */ 
#define IDMSG2           Id_Array[NUMCUR][6]       /* 电池模块消息2             */ 
#define IDVER            Id_ArrayVer[NUMCUR]       /* 电池模块版本信息          */
#define PRTY_TEMP        4
#define IDCANCCP         Id_ArrayCcp[NUMCUR]       /* LECU CAN的CCP建立ID       */
/*********************************Function declaration****************************************/
void CanRxManage(void);
void CanTxManage(void);
void Can_DataLoad(void);
void CellVolLoad(void);
void TempLoad(void);
void Msg1Load(void);
void Msg2Load(void);
void VerLoad(void);

#endif