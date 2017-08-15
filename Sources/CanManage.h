#ifndef __CANMANAGE_H_
#define __CANMANAGE_H_  

#include "INCLUDES.h"
/***************************************Type define*******************************************/
typedef struct LECURX_MSG
{
    INT32U  EquTime;        /* ��λΪ0.5s */
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
#define IDCELLVOL_N1     Id_Array[NUMCUR][0]       /* ����ص�ѹ��Ϣ1           */
#define IDCELLVOL_N2     Id_Array[NUMCUR][1]       /* ����ص�ѹ��Ϣ2           */
#define IDCELLVOL_N3     Id_Array[NUMCUR][2]       /* ����ص�ѹ��Ϣ3           */
#define IDCELLVOL_N4     Id_Array[NUMCUR][3]       /* ����ص�ѹ��Ϣ4           */
#define IDTEMP           Id_Array[NUMCUR][4]       /* �¶���Ϣ                  */ 
#define IDMSG1           Id_Array[NUMCUR][5]       /* ���ģ����Ϣ1             */ 
#define IDMSG2           Id_Array[NUMCUR][6]       /* ���ģ����Ϣ2             */ 
#define IDVER            Id_ArrayVer[NUMCUR]       /* ���ģ��汾��Ϣ          */
#define PRTY_TEMP        4
#define IDCANCCP         Id_ArrayCcp[NUMCUR]       /* LECU CAN��CCP����ID       */
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