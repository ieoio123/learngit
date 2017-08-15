/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     CanManage.c
*Programmer(s) : Zeng Qunxin
*Descriprion :    
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "INCLUDES.h"

/************************************const value define***************************************/
/* 详细信息数据帧ID表 */
const INT16U Id_Array[NUMMODULE][7] = 
{  
    {0x101,0x102,0x103,0x104,0x105,0x109,0x10D}, //0
    
    /*{0x111,0x112,0x113,0x114,0x115,0x119,0x11D},
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
    {0x2D1,0x2D2,0x2D3,0x2D4,0x2D5,0x2D9,0x2DD}  //24 */
};
const INT16U Id_ArrayCcp[25] = 
{
    0x450,                                                               //0
    0x451,0x452,0x453,0x454,0x455,0x456,0x457,0x458,0x459,0x45A,0x45B,   //11
    0x461,0x462,0x463,0x464,0x465,0x466,0x467,0x468,0x469,0x46A,0x46B,   //22
    0x46C,0x46D  
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
    "HW:RL14V19L3X1307,SW:ZT12m-L.A-V3.0.006(V5.1),PR:ZT12m,AU:zengqx,DT:2013.08.19"
};
/************************************Variable define******************************************/
CANTX_MSG CanTxInfoBuf[7];
CANTX_MSG CanTxVerBuf;  
LECURX_MSG LecuRxMsg;
static INT8U sCnt = 0;
static BOOLEAN sVerFlg = FALSE;
static BOOLEAN sVerEnd = FALSE; 
/********************************Extern variable declaration**********************************/
extern LECU_INFO LecuInfo;
extern CANTX_MSG *CanTxBuf;                                            /* 待发送信息存储区 */
/* Read Only */
extern CANRX_MSG CanRxBuf[4]; 
/*
************************************************************************************************
*  Function:         CanRxManage()
*  Description:      CAN接收数据管理函数
*  Calls:            EE_DwordErase();EE_WordWrite();
*  Input:            
*  Output:           无 
*  Return:           无 
*  Others:                          
************************************************************************************************
*/
void CanRxManage(void)     
{
    INT8U i = 0;
    INT8U *p_data;
    CANRX_MSG tmpbuf;
    static INT8U sTime500msCnt = 0; 
       
    for(i = 0; i < 4; i++)
    {
        if(CanRxBuf[i].newdata == TRUE)
        {                                      
            tmpbuf = CanRxBuf[i];
            CanRxBuf[i].newdata = FALSE;
            p_data = &tmpbuf.data[0];
            switch(tmpbuf.id)
            {
                case IDCANCMD:
                {
                    #define EQUTIME     (*(p_data + 0))
                    #define EQUCMD      (*(p_data + 1)) 
                    #define EQUVOL      (*(INT16U *)(p_data + 2))
                    #define CURRENT     (*(INT16S *)(p_data + 4)) 
                    #define FANCTRL     (*(p_data + 6)) 
                    LecuRxMsg.EquTime = EQUTIME * 36;//3600;  /* 单位为0.5s */
                    LecuRxMsg.EquCmd = EQUCMD;
                    LecuRxMsg.EquVol = EQUVOL;
                    LecuRxMsg.Cur = CURRENT;
                    LecuRxMsg.FanCtrl = FANCTRL;
                    LecuRxMsg.CommTimeOut = FALSE;
                    sTime500msCnt = 0;               
                    #undef  FANCTRL
                    #undef  CURRENT
                    #undef  EQUVOL
                    #undef  EQUCMD
                    #undef  EQUTIME                   
                    break;    
                }
                case IDCANDIAG:
                {
                    #define LECU_CODE   (*(INT16U *)(p_data + 0))
                    #define EQU_NO      (*(p_data + 2))
                    #define EQU_CH      (*(INT16U *)(p_data + 3))
                    #define EQU_DIR     (*(p_data + 4) >> 6)
                    #define FAN_CTL     (*(p_data + 5))
                    #define OLD_ID      (*(p_data + 6))
                    #define NEW_ID      (*(p_data + 7))
                    if(LECU_CODE == DIAG_CODE)
                    {                        
                        if((NUMCUR == OLD_ID) && (NEW_ID <= NUMMODULE))
                        {
                            (void)EE_DwordErase(0x0708);
                            (void)EE_WordWrite(0x0708, NEW_ID);
                        }
                        if((NUMCUR > 0) && (NUMCUR <= NUMMODULE))
                        {
                            LecuRxMsg.DiagFlg = TRUE;
                            LecuRxMsg.DiagFanCtrl = FAN_CTL;
                            if(NUMCUR == EQU_NO)
                            {
                                LecuRxMsg.DiagEquCh  = ((EQU_CH << 8) | (EQU_CH >> 8)) & PASSIVE_BLMASK;
                                LecuRxMsg.DiagEquDir = EQU_DIR;                                
                            }
                              
                        }else
                        {
                            LecuRxMsg.DiagFlg = FALSE;
                        }
                        
                    }else
                    {
                        LecuRxMsg.DiagFlg = FALSE;
                        LecuRxMsg.DiagEquCh = 0;
                        LecuRxMsg.DiagEquDir = 0;
                        LecuRxMsg.DiagFanCtrl = FALSE;
                    }
                    #undef NEW_ID
                    #undef OLD_ID                   
                    #undef FAN_CTL
                    #undef EQU_DIR
                    #undef EQU_CH
                    #undef EQU_NO                    
                    #undef LECU_CODE
                    break;
                }
                case IDCANQUERY:
                {
                    LecuRxMsg.VerFlg = TRUE;
                    break;
                }
                default:
                {
                    if(tmpbuf.id == IDCANCCP)
                    {
                        #define CCP_CMD     (*(INT16U *)(p_data+0)) 
                        if(CCP_CMD == CCP_CODE)
                        {
                            if(0xFFFF != *(INT16U *)0xEFFE)         /* 确认App通过Bootloader烧写*/
                            {
                                (void)EE_DwordErase(0x0720);
                                (void)EE_WordWrite(0x0720,0);
                                asm call 0xF000,PPAGE                 /* 转入Bootloader程序 */
                            }     
                        } 
                        #undef CCP_CMD
                    }
                    break; 
                }
                
            }
        }
    }
    if(sTime500msCnt > 6)
    {
        LecuRxMsg.CommTimeOut = TRUE;        
    }else
    {
        sTime500msCnt++;
    }
}
/*
************************************************************************************************
*  Function:         CanTxManage()
*  Description:      CAN发送数据管理函数
*  Calls:            CanTxStatusCheck();CanLoadStdMsg();
*  Input:            
*  Output:           无 
*  Return:           无 
*  Others:                          
************************************************************************************************
*/
void CanTxManage(void)
{
    BOOLEAN start_cantx = FALSE;
    INT8U   tx_ch = 0;
    
    CANTIER = 0;
    LecuInfo.WorkMode = DIAG;
    switch(LecuInfo.WorkMode)
    {
        case DIAG: 
        {
            sCnt++;
            /* 发送优先级设置 */
            CanTxInfoBuf[0].prty = 0;                   
            CanTxInfoBuf[1].prty = 1;
            CanTxInfoBuf[2].prty = 2;
            CanTxInfoBuf[3].prty = 3;
            CanTxInfoBuf[4].prty = 4;
            CanTxInfoBuf[5].prty = 5;
            CanTxInfoBuf[6].prty = 6;
            /* 发送数据串接 */
            CanTxInfoBuf[0].next = &CanTxInfoBuf[1];
            CanTxInfoBuf[1].next = &CanTxInfoBuf[2];
            CanTxInfoBuf[2].next = &CanTxInfoBuf[3];
            CanTxInfoBuf[3].next = &CanTxInfoBuf[4];
            CanTxInfoBuf[4].next = &CanTxInfoBuf[5];     
            CanTxInfoBuf[5].next = &CanTxInfoBuf[6];
            CanTxInfoBuf[6].next = NULL;                
            CanTxBuf = &CanTxInfoBuf[0];
            start_cantx = TRUE;
            break; 
        }
        case WORK:
        {
            sCnt++;
            if(LecuRxMsg.EquCmd == 0)
            {   
                /* 发送优先级设置 */         
                CanTxInfoBuf[5].prty = 0;
                CanTxInfoBuf[6].prty = 1;
                /* 发送数据串接 */
                CanTxInfoBuf[5].next = &CanTxInfoBuf[6];
                CanTxInfoBuf[6].next = NULL;
                CanTxBuf = &CanTxInfoBuf[5];
                start_cantx = TRUE; 
            }else 
            {
                /* 发送优先级设置 */
                CanTxInfoBuf[0].prty = 0;                   
                CanTxInfoBuf[1].prty = 1;
                CanTxInfoBuf[2].prty = 2;
                CanTxInfoBuf[3].prty = 3;
                CanTxInfoBuf[4].prty = 4;
                CanTxInfoBuf[5].prty = 5;
                CanTxInfoBuf[6].prty = 6;
                /* 发送数据串接 */
                CanTxInfoBuf[0].next = &CanTxInfoBuf[1];
                CanTxInfoBuf[1].next = &CanTxInfoBuf[2];
                CanTxInfoBuf[2].next = &CanTxInfoBuf[3];
                CanTxInfoBuf[3].next = &CanTxInfoBuf[4];
                CanTxInfoBuf[4].next = &CanTxInfoBuf[5];     
                CanTxInfoBuf[5].next = &CanTxInfoBuf[6];
                CanTxInfoBuf[6].next = NULL;                
                CanTxBuf = &CanTxInfoBuf[0];
                start_cantx = TRUE; 
            } 
            break; 
        }
        default:
        {
            break;
        }  
    }
    if(sVerFlg == TRUE)
    {       
        if(start_cantx == TRUE)
        {
            CanTxVerBuf.next = CanTxBuf;
        }
        CanTxBuf = &CanTxVerBuf;                     
        start_cantx = TRUE;  
        if(sVerEnd == TRUE)
        {   
            LecuRxMsg.VerFlg = FALSE; 
            sVerFlg = FALSE;              
        }
    }
    
    if(start_cantx == TRUE)                    /* 启动CAN数据发送 */
    {
        tx_ch = CanTxStatusCheck();            /* 寻找一个空闲CAN发送通道*/
        if(tx_ch != 0)
        {
            CanLoadStdMsg(CanTxBuf);           /* 装载数据至待发送区 */
            CANTFLG = tx_ch;                   /* 开始发送 */
            CANTIER = 0x07;                    /* 开启发送缓冲区空中断 */
        }
    }
}
/*
************************************************************************************************
*  Function:         Can_DataLoad()
*  Description:      CAN发送数据装载   
*  Calls:            Msg1Load();Msg2Load();CellVolLoad();TempLoad();VerLoad();
*  Input:            无
*  Output:           无 
*  Return:           无
*  Others:                         
************************************************************************************************
*/
void Can_DataLoad(void)
{  
    Msg1Load();
    Msg2Load();
    CellVolLoad();
    TempLoad();
    VerLoad();
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
    INT16U *p_vol = LecuInfo.CellVol;
    INT32U time_sum = 0;
    CanTxInfoBuf[i + 0].id = IDCELLVOL_N1;   
    CanTxInfoBuf[i + 1].id = IDCELLVOL_N2;
    CanTxInfoBuf[i + 2].id = IDCELLVOL_N3;
    CanTxInfoBuf[i + 3].id = IDCELLVOL_N4;
    
    for(i = 0, index = 0; ((i < 4) || (index < NUMCELL)); i++)
    {
        if(index < NUMCELL)    
        {
            CanTxInfoBuf[i].data[0] = (INT8U)((*p_vol) >> 8);  
            CanTxInfoBuf[i].data[1] = (INT8U)(*p_vol);        
            index++;
            p_vol++;
        }else
        {
            CanTxInfoBuf[i].data[0] = 0;
            CanTxInfoBuf[i].data[1] = 0;
        }
        if(index < NUMCELL)    
        {
            CanTxInfoBuf[i].data[2] = (INT8U)((*p_vol) >> 8); 
            CanTxInfoBuf[i].data[3] = (INT8U)(*p_vol);         
            index++;
            p_vol++;
        }else
        {
            CanTxInfoBuf[i].data[2] = 0;
            CanTxInfoBuf[i].data[3] = 0;
        }
        if(index < NUMCELL)    
        {
            CanTxInfoBuf[i].data[4] = (INT8U)((*p_vol) >> 8);  
            CanTxInfoBuf[i].data[5] = (INT8U)(*p_vol);         
            index++;
            p_vol++;
        }else
        {
            CanTxInfoBuf[i].data[4] = 0;
            CanTxInfoBuf[i].data[5] = 0;
        }
        if(index < NUMCELL)    
        {
            CanTxInfoBuf[i].data[6] = (INT8U)((*p_vol) >> 8);  
            CanTxInfoBuf[i].data[7] = (INT8U)(*p_vol);         
            index++;
            p_vol++;
        }else
        {
            CanTxInfoBuf[i].data[6] = 0;
            CanTxInfoBuf[i].data[7] = 0;
        }
        CanTxInfoBuf[i].len = 8;
    }
    time_sum = EE_WordRead(0x0730);
    time_sum = (time_sum << 16) | EE_WordRead(0x0732); /* 读取单位为0.5s */
    time_sum = time_sum / 3600;                /* 转换单位为1min */ 
    CanTxInfoBuf[3].data[6] = (INT8U)(time_sum >> 8);
    CanTxInfoBuf[3].data[7] = (INT8U)(time_sum);
       
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
    INT8U j = 0;  
    INT32U blch = 0;
 
    blch = (INT16U)LecuInfo.BlCh;
 
    CanTxInfoBuf[i].id = IDTEMP;
    CanTxInfoBuf[i].prty = PRTY_TEMP; 
            
    for(j = 0; j < 4; j++)
    {   
        if(j < NUMTEMP)
        {
            CanTxInfoBuf[i].data[j] = LecuInfo.Temp[j];
        }else
        {
            CanTxInfoBuf[i].data[j] = 0;
        }       
    }
    CanTxInfoBuf[i].data[4] = LecuInfo.PcbTemp; 
    CanTxInfoBuf[i].data[5] = (INT8U)(blch >> 8);                               
    CanTxInfoBuf[i].data[6] = (INT8U)blch;
    CanTxInfoBuf[i].data[7] = 0;
    CanTxInfoBuf[i].len = 8;
    CanTxInfoBuf[i].rtr = FALSE;     
}
/*
************************************************************************************************
*  Function:         Msg1VLoad()
*  Description:      LECU简要信息1数据装载到发送缓冲区
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/
void Msg1Load(void)
{
    INT8U i = 5;
      
    CanTxInfoBuf[i].id = IDMSG1;
    CanTxInfoBuf[i].prty = 0;
    
    CanTxInfoBuf[i].data[0] = (INT8U)(LecuInfo.MaxCellVol >> 8);
    CanTxInfoBuf[i].data[1] = (INT8U)LecuInfo.MaxCellVol;
    CanTxInfoBuf[i].data[2] = (INT8U)(LecuInfo.MinCellVol >> 8);
    CanTxInfoBuf[i].data[3] = (INT8U)LecuInfo.MinCellVol;    
    CanTxInfoBuf[i].data[4] = LecuInfo.MaxTemp; 
    CanTxInfoBuf[i].data[5] = LecuInfo.MinTemp;                               
    CanTxInfoBuf[i].data[6] = (LecuInfo.MinCellVolIndex << 4) | (LecuInfo.MaxCellVolIndex);
    CanTxInfoBuf[i].data[7] = sCnt;    
    CanTxInfoBuf[i].len = 8;
    CanTxInfoBuf[i].rtr = FALSE; 
       
}
/*
************************************************************************************************
*  Function:         Msg2VLoad()
*  Description:      LECU简要信息2数据装载到发送缓冲区
*  Calls:            无
*  Input:            无
*  Output:           无
*  Return:           无
*  Others:           无                 
************************************************************************************************
*/
void Msg2Load(void)
{
    INT8U i = 6;
      
    CanTxInfoBuf[i].id = IDMSG2;
    CanTxInfoBuf[i].prty = 0;
    
    CanTxInfoBuf[i].data[0] = (INT8U)(LecuInfo.ModuleVol >> 8);
    CanTxInfoBuf[i].data[1] = (INT8U)LecuInfo.ModuleVol;
    CanTxInfoBuf[i].data[2] = (INT8U)(LecuInfo.ErrCode >> 8);
    CanTxInfoBuf[i].data[3] = (INT8U)LecuInfo.ErrCode;    
    CanTxInfoBuf[i].data[4] = LecuInfo.MaxInnerRes;
    CanTxInfoBuf[i].data[5] = LecuInfo.MinInnerRes;                               
    CanTxInfoBuf[i].data[6] = (LecuInfo.MaxInnerResIndex << 4) | (LecuInfo.MaxInnerResIndex);
    CanTxInfoBuf[i].data[7] = ((sCnt & 0x0F) << 4) | LecuInfo.RunMode;
   
    CanTxInfoBuf[i].len = 8;
    CanTxInfoBuf[i].rtr = FALSE; 
       
}
/*
************************************************************************************************
*  Function:         VerLoad()
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
    static INT8U s_vercnt = 0;
    static INT8U *p_str = VerString;
    INT8U i = 0;
    
    if((sVerFlg == FALSE) && (LecuRxMsg.VerFlg == TRUE))
    {
        sVerFlg = TRUE;
        p_str = VerString;
        s_vercnt = 0;
        sVerEnd = FALSE;              
    }                                              /* 初始化字符串显示位置 */
    if(sVerFlg == TRUE)
    {
        CanTxVerBuf.id = IDVER;
        for(i = 0; i < 7; i++)
        {
            if(*p_str != '\0')                     /* 字符串结束 */
            {
                CanTxVerBuf.data[i] = *p_str;   /* 载入字符 */
                p_str++;
            }else
            {
                CanTxVerBuf.data[i] = 0;
                sVerEnd = TRUE;
            }
        }
        CanTxVerBuf.data[7] = s_vercnt++;   
        CanTxVerBuf.len = 8;
        CanTxVerBuf.rtr = FALSE;
        CanTxVerBuf.next = NULL;
    }
} 