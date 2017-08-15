/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     DataLoad.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   数据装载
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "INCLUDES.h"  
/************************************const value define***************************************/
/* 详细信息数据帧ID表 */
const INT16U Id_Array[25][7] = 
{  
    {0x101,0x102,0x103,0x104,0x105,0x109,0x10D}, //0
    
    {0x111,0x112,0x113,0x114,0x115,0x119,0x11D},
    {0x121,0x122,0x123,0x124,0x125,0x129,0x12D},
    {0x131,0x132,0x133,0x134,0x135,0x139,0x13D},
    {0x141,0x142,0x143,0x144,0x145,0x149,0x14D},   
    {0x151,0x152,0x153,0x154,0x155,0x159,0x15D},
    {0x161,0x162,0x163,0x164,0x165,0x169,0x16D},
    {0x171,0x172,0x173,0x174,0x175,0x179,0x17D},
    {0x181,0x182,0x183,0x184,0x185,0x189,0x18D},                       
    {0x191,0x192,0x193,0x194,0x195,0x199,0x19D},
    {0x1A1,0x1A2,0x1A3,0x1A4,0x1A5,0x1A9,0x1AD},
    {0x1B1,0x1B2,0x1B3,0x1B4,0x1B5,0x1B9,0x1BD}, //11

    {0x211,0x212,0x213,0x214,0x215,0x219,0x21D}, 
    {0x221,0x222,0x223,0x224,0x225,0x229,0x22D},
    {0x231,0x232,0x233,0x234,0x235,0x239,0x23D},
    {0x241,0x242,0x243,0x244,0x245,0x249,0x24D},   
    {0x251,0x252,0x253,0x254,0x255,0x259,0x25D},
    {0x261,0x262,0x263,0x264,0x265,0x269,0x26D},
    {0x271,0x272,0x273,0x274,0x275,0x279,0x27D},
    {0x281,0x282,0x283,0x284,0x285,0x289,0x28D},                       
    {0x291,0x292,0x293,0x294,0x295,0x299,0x29D},
    {0x2A1,0x2A2,0x2A3,0x2A4,0x2A5,0x2A9,0x2AD},
    {0x2B1,0x2B2,0x2B3,0x2B4,0x2B5,0x2B9,0x2BD}, //22

    {0x2C1,0x2C2,0x2C3,0x2C4,0x2C5,0x2C9,0x2CD},
    {0x2D1,0x2D2,0x2D3,0x2D4,0x2D5,0x2D9,0x2DD}  //24
};
const INT16U Id_ArrayVer[25] = 
{
    0x030,                                                               //0
    0x031,0x032,0x033,0x034,0x035,0x036,0x037,0x038,0x039,0x03A,0x03B,   //11
    0x041,0x042,0x043,0x044,0x045,0x046,0x047,0x048,0x049,0x04A,0x04B,   //22
    0x04C,0x04D                                                          //24
};
const INT8U VerString[] = 
{
    "Ver: 1.0\nAuth: ZengQX\nData: 20120413\nMcu: G128\nChip: LTC6803-1\nPrjt: Audi\nBoot: 1.0"
};
/********************************Extern variable declaration**********************************/
extern CANTX_MSG Info_SendBuf[NUMFRAME];             /* 待发送信息存储区 */
extern INT16U  Info_CellVol[NUMCELL];              /* 单体电压数据存储区 */
extern INT8U   Info_Temp[NUMTEMP];                 /* 温度数据存储区 */
extern INT32U  App_TxBlCh;                         /* 发送的均衡通道值 */
extern INT8U   Info_PcbTemp;   
extern INT16U Info_MaxCellVol;
extern INT16U Info_MinCellVol;
extern INT8U  Info_MaxCellVolIndex;
extern INT8U  Info_MinCellVolIndex;
extern INT8U  Info_MaxTemp;
extern INT8U  Info_MinTemp;
extern INT8U  Info_MaxTempIndex;
extern INT8U  Info_MinTempIndex;
extern INT16U Info_ModuleVol;
extern INT16U Diag_ErrCode;
/*
************************************************************************************************
*  Function:         Info_DataLoad()
*  Description:      发送数据装载
*  Calls:            CellVolLoad();TempLoad();
*  Input:            无
*  Output:           无 
*  Return:           无
*  Others:           电压，温度，状态等数据装载                  
************************************************************************************************
*/
void Info_DataLoad(void)
{  
    Msg1Load();
    Msg2Load();
    CellVolLoad();
    TempLoad();
}
/*
************************************************************************************************
*  Function:         CellVolLoad()
*  Description:      电压数据装载到发送缓冲区
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/
void CellVolLoad(void)
{
    INT8U i = 0;
    INT8U index = 0;
    INT16U *p_vol = Info_CellVol;
    
    Info_SendBuf[i + 0].id = IDCELLVOL_N1;
    Info_SendBuf[i + 0].prty = PRTY_CELLVOL_N1;    
    Info_SendBuf[i + 1].id = IDCELLVOL_N2;
    Info_SendBuf[i + 0].prty = PRTY_CELLVOL_N2; 
    Info_SendBuf[i + 2].id = IDCELLVOL_N3;
    Info_SendBuf[i + 0].prty = PRTY_CELLVOL_N3; 
    Info_SendBuf[i + 3].id = IDCELLVOL_N4;
    Info_SendBuf[i + 0].prty = PRTY_CELLVOL_N4; 
    
    for(i = 0, index = 0; ((i < 4) || (index < NUMCELL)); i++)
    {
        if(index < NUMCELL)    
        {
            Info_SendBuf[i].data[0] = (*p_vol) >> 8; //H
            Info_SendBuf[i].data[1] = (*p_vol);      //L
            index++;
            p_vol++;
        }else
        {
            Info_SendBuf[i].data[0] = 0;
            Info_SendBuf[i].data[1] = 0;
        }
        if(index < NUMCELL)    
        {
            Info_SendBuf[i].data[2] = (*p_vol) >> 8; //H
            Info_SendBuf[i].data[3] = (*p_vol);      //L
            index++;
            p_vol++;
        }else
        {
            Info_SendBuf[i].data[0] = 0;
            Info_SendBuf[i].data[1] = 0;
        }
        if(index < NUMCELL)    
        {
            Info_SendBuf[i].data[4] = (*p_vol) >> 8; //H
            Info_SendBuf[i].data[5] = (*p_vol);      //L
            index++;
            p_vol++;
        }else
        {
            Info_SendBuf[i].data[0] = 0;
            Info_SendBuf[i].data[1] = 0;
        }
        Info_SendBuf[i].data[6] = 0;   
/*      Info_SendBuf[i].data[7] = 0;                */  
        Info_SendBuf[i].len = 8;
    }   
}

/*
************************************************************************************************
*  Function:         TempLoad()
*  Description:      温度数据和均衡通道信息装载到发送缓冲区
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/   
void TempLoad(void)
{
    INT8U i = 4;
      
    Info_SendBuf[i].id = IDTEMP;
    Info_SendBuf[i].prty = PRTY_TEMP; 
            
    Info_SendBuf[i].data[0] = Info_Temp[0];
    Info_SendBuf[i].data[1] = Info_Temp[1];
    Info_SendBuf[i].data[2] = Info_Temp[2];
    Info_SendBuf[i].data[3] = Info_PcbTemp;    
    Info_SendBuf[i].data[4] = (INT8U)(App_TxBlCh>>24); 
    Info_SendBuf[i].data[5] = (INT8U)(App_TxBlCh>>16);                               
    Info_SendBuf[i].data[6] = (INT8U)(App_TxBlCh>>8);
    Info_SendBuf[i].data[7] = (INT8U)App_TxBlCh;
    Info_SendBuf[i].len = 8;
    Info_SendBuf[i].rtr = FALSE;     
}
/*
************************************************************************************************
*  Function:         TempLoad()
*  Description:      版本信息装载
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/   
void VerLoad(void)
{
    INT8U i = 6;
      
    Info_SendBuf[i].id = IDVER;
    Info_SendBuf[i].prty = 0;
    
    Info_SendBuf[i].data[0] = Info_Temp[0];
    Info_SendBuf[i].data[1] = Info_Temp[1];
    Info_SendBuf[i].data[2] = Info_Temp[2];
    Info_SendBuf[i].data[3] = Info_PcbTemp;    
    Info_SendBuf[i].data[4] = (INT8U)(App_TxBlCh>>24); 
    Info_SendBuf[i].data[5] = (INT8U)(App_TxBlCh>>16);                               
    Info_SendBuf[i].data[6] = (INT8U)(App_TxBlCh>>8);
    Info_SendBuf[i].data[7] = (INT8U)App_TxBlCh;
    Info_SendBuf[i].len = 8;
    Info_SendBuf[i].rtr = FALSE;   
} 
/*
************************************************************************************************
*  Function:         Msg1VLoad()
*  Description:      
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/
void Msg1Load(void)
{
    INT8U i = 4;
      
    Info_SendBuf[i].id = IDMSG1;
    Info_SendBuf[i].prty = 0;
    
    Info_SendBuf[i].data[0] = (Info_MaxCellVol >> 8);
    Info_SendBuf[i].data[1] = Info_MaxCellVol;
    Info_SendBuf[i].data[2] = (Info_MinCellVol >> 8);
    Info_SendBuf[i].data[3] = Info_MinCellVol;    
    Info_SendBuf[i].data[4] = Info_MaxTemp; 
    Info_SendBuf[i].data[5] = Info_MinTemp;                               
    Info_SendBuf[i].data[6] = (Info_MinCellVolIndex << 4) | (Info_MaxCellVolIndex);
/*  Info_SendBuf[i].data[7] = 0;    */
    Info_SendBuf[i].len = 8;
    Info_SendBuf[i].rtr = FALSE; 
       
}
/*
************************************************************************************************
*  Function:         Msg2VLoad()
*  Description:      
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/
void Msg2Load(void)
{
    INT8U i = 5;
      
    Info_SendBuf[i].id = IDMSG2;
    Info_SendBuf[i].prty = 0;
    
    Info_SendBuf[i].data[0] = (Info_ModuleVol >> 8);
    Info_SendBuf[i].data[1] = Info_ModuleVol;
    Info_SendBuf[i].data[2] = (Diag_ErrCode >> 8);
    Info_SendBuf[i].data[3] = Diag_ErrCode;    
    Info_SendBuf[i].data[4] = 0; /* 最大内阻 */
    Info_SendBuf[i].data[5] = 0;                               
    Info_SendBuf[i].data[6] = 0;
    Info_SendBuf[i].data[7] = 0;
    Info_SendBuf[i].len = 8;
    Info_SendBuf[i].rtr = FALSE; 
       
} 
        