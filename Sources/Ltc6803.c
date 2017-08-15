/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     Ltc6803.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   两片Ltc6803的相关操作 
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "Ltc6803.h"
/************************************const value define***************************************/
const INT8U Crc107_HalfList[]=                   /* Crc107半字节查表数组 */
{
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
    0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D
};
/************************************Variable define******************************************/
static BOOLEAN sLtc6803Status = FALSE;           /* Ltc6803设备状态 */
static INT32U  sLtc6803Blch = 0;                 /* Ltc6803均衡状态 */
/*
************************************************************************************************
*  Function:         Ltc6803Devstatus()
*  Description:      获取Lct6803的当前设备状态
*  Calls:            
*  Input:           
*  Output:           无 
*  Return:           BOOLEAN
*  Others:           返回Ltc6803的设备状态,
*                    TRUE--设备正常,FALSE--设备工作异常        
************************************************************************************************
*/
BOOLEAN Ltc6803Devstatus(void)
{
    return sLtc6803Status;
}
/*
************************************************************************************************
*  Function:         Ltc6803CfgInit()
*  Description:      Lct6803的配置寄存器初始化
*  Calls:            Ltc6803Cfg();
*  Input:            INT32U blch
*  Output:           无 
*  Return:           BOOLEAN
*  Others:           返回Ltc6803的设备状态,
*                    TRUE--设备正常,FALSE--设备工作异常        
************************************************************************************************
*/
BOOLEAN Ltc6803CfgInit(INT32U blch)
{
    sLtc6803Status = Ltc6803Cfg(blch);                /* 配置Lct6803寄存器   */
    return sLtc6803Status;
}
/*
************************************************************************************************
*  Function:         Ltc6803CellVolDetect()
*  Description:      Lct6803的电压采样函数
*  Calls:            Ltc6803Wr();MCG_DelayMs();Ltc6803Rd()
*  Input:            INT16U *p_vcell
*  Output:           无 
*  Return:           BOOLEAN
*  Others:           输入参数p_vcell为电压存储数组首地址,
*                    返回Ltc6803的设备状态,TRUE--设备正常,FALSE--设备工作异常        
************************************************************************************************
*/
BOOLEAN Ltc6803CellVolDetect(INT16U *p_vcell)
{
    INT32U blch = sLtc6803Blch;     
    
    sLtc6803Status = Ltc6803Cfg(0);                   /* 配置Lct6802寄存器    */
    Ltc6803Wr(STCVAD);                                /* 发起12节正常采样命令 */
    MCG_DelayMs(15);                                  /* 等待采样完成         */
    Ltc6803Rd(p_vcell);                               /* 读取采样电压值       */
    sLtc6803Status = Ltc6803Cfg(blch);                /* 配置Lct6802寄存器    */
    return sLtc6803Status;
}
/*
************************************************************************************************
*  Function:         Ltc6803OpenDetect()
*  Description:      电池采样的断线检测
*  Calls:            Ltc6803Wr();MCG_DelayMs();Ltc6803Rd()
*  Input:            
*  Output:           无 
*  Return:           INT8U
*  Others:           返回检测状态      
************************************************************************************************
*/
BOOLEAN Ltc6803OpenDetect(void)
{    
    INT8U i;
    INT16S openvcell[CELLNUM];                        /* 断线检测电压数组 */
    
    (void)Ltc6803Wr(STOWAD);                          /* 发起12节开线采样命令 */
    MCG_DelayMs(15);                                  /* 等待采样完成         */
    Ltc6803Rd((INT16U *)openvcell);                             /* 读取采样电压值       */
     
    if(sLtc6803Status == FALSE)
    {
        return FALSE;
    }
    for(i = 0; i < CELLNUM; i++)
    {
        if(openvcell[i] < OPEN_VOL_DIFF)              /* 电压值小于断线阈值   */
        {
            return FALSE;                             
        }
    }                                                 
    return TRUE;                                      /* 返回未发生断线 */
}
/*
************************************************************************************************
*  Function:         Ltc6803TempDectect()
*  Description:      电池采样的断线检测
*  Calls:            Ltc6803Wr();MCG_DelayMs();Ltc6803Rd()
*  Input:            INT8U *p_temp
*  Output:           无 
*  Return:           INT8U
*  Others:           输入参数p_temp为温度存储数组首地址     
************************************************************************************************
*/
void Ltc6803TempDectect(INT8U *p_temp)
{
    (void)Ltc6803Wr(STTMPAD);                         /* 发起芯片内部温度采样命令 */
    MCG_DelayMs(10);                                  /* 等待采样完成             */
    *p_temp = Ltc6803RdTemp();                        /* 获取采样温度结果         */
}
/*
************************************************************************************************
*  Function:         Ltc6803Cfg()
*  Description:      配置寄存器,检测设备状态
*  Calls:            Ltc6803Wr();Crc107();
*  Input:            INT32U blchannel
*  Output:           无 
*  Return:           BOOLEAN
*  Others:           blchannel芯片管理均衡通道值，
*                    返回Ltc6803的设备状态,TRUE--设备正常,FALSE--设备工作异常                        
************************************************************************************************
*/
BOOLEAN Ltc6803Cfg(INT32U blchannel) 
{                      
    INT8U i;                               
    INT8U wrregdata[2][7];                                 /* Ltc6803寄存器配置写数组*/
    INT8U rdregdata[2][7];                                 /* Ltc6803寄存器配置读数组*/
    INT8U rddata = 0;
    INT16U bl_top = (blchannel >> CELLNUM_BOTTOM) & NUMMASK_TOP;
    INT16U bl_bottom = blchannel & NUMMASK_BOTTOM; 
    INT8U blch_h_bottom = (INT8U)(bl_bottom >> 8) & 0xF0;
    INT8U blch_l_bottom = (INT8U)bl_bottom;
    INT8U blch_h_top = (INT8U)(bl_top >> 8) & 0xF0;     /* Bleed Channel: 12 ~ 9 */ 
    INT8U blch_l_top = (INT8U)bl_top;                   /* Bleed Channel:  8 ~ 1 */
    INT8U mask_ir_h = (INT8U)(MASK_CELL_IR >> 4);       /* Mask Cell 12~5 Interrupts */ 
    INT8U mask_ir_l = (INT8U)(MASK_CELL_IR & 0x0F);     /* Mask Cell  4~1 Interrupts */  
    
    /* Top Registers */                                               
    wrregdata[0][0]  = 0x91;                      /* enable watchdog,level polling,12-cell,standby mode */ 
    wrregdata[0][1]  = blch_l_top;                /* Bleed Channel: 8 ~ 1                               */
    wrregdata[0][2]  = blch_h_top | mask_ir_l;    /* Mask Cell Interrupts: 4 ~ 1, Bleed Channel: 12 ~ 9 */   
    wrregdata[0][3]  = mask_ir_h;                 /* Mask Cell Interrupts: 12 ~ 5                       */ 
    wrregdata[0][4]  = 0x49;                      /* VUV=0x2A*16*1.5mV=1.008V                           */
    wrregdata[0][5]  = 0xE0;                      /* VOV=0xD0*16*1.5mV=4.992V                           */ 
    wrregdata[0][6]  = Crc107(&wrregdata[0][0],6);/* 获取写寄存器信息的CRC码 */ 
    /* Bottom Registers */
    wrregdata[1][0]  = 0x91;                      /* enable watchdog,level polling,12-cell,standby mode */ 
    wrregdata[1][1]  = blch_l_bottom;             /* Bleed Channel: 8 ~ 1                               */
    wrregdata[1][2]  = blch_h_bottom | mask_ir_l; /* Mask Cell Interrupts: 4 ~ 1, Bleed Channel: 12 ~ 9 */   
    wrregdata[1][3]  = mask_ir_h;                 /* Mask Cell Interrupts: 12 ~ 5                       */ 
    wrregdata[1][4]  = 0x49;                      /* VUV=0x2A*16*1.5mV=1.008V                           */
    wrregdata[1][5]  = 0xE0;                      /* VOV=0xD0*16*1.5mV=4.992V                           */ 
    wrregdata[1][6]  = Crc107(&wrregdata[1][0],6);/* 获取写寄存器信息的CRC码 */ 
    
    (void)Ltc6803Wr(WRCFG);    
    LTC6803_SS = SPI_ON;                      /* SPI通信打开 */                              
    for(i = 0; i < 7;i ++)
    {
        LTC6803_COMM(wrregdata[0][i], rddata);   /* 发送配置寄存器信息 */
    }                          
    for(i = 0; i < 7;i ++)
    {
        LTC6803_COMM(wrregdata[1][i], rddata);   /* 发送配置寄存器信息 */
    }
    LTC6803_SS = SPI_OFF;                     /* SPI通信关闭 */ 
    
    LTC6803_SS = SPI_ON;                      /* SPI通信打开 */                            
    (void)Ltc6803Wr(RDCFG);
    for(i=0;i<7;i++)
    {
        LTC6803_COMM(0x00,rdregdata[1][i]);      /* 读取配置寄存器信息 */
    }
    for(i=0;i<7;i++)
    {
        LTC6803_COMM(0x00,rdregdata[0][i]);      /* 读取配置寄存器信息 */
    }
    LTC6803_SS = SPI_OFF;                     /* SPI通信关闭 */ 
    
    if((rdregdata[0][6] != Crc107(&rdregdata[0][0],6)) 
    || (rdregdata[1][6] != Crc107(&rdregdata[1][0],6))) 
    {        
        //return FALSE;                         /* CRC校验失败 */
    }else
    {    
        for(i = 0; i < 6; i++)
        {
            if(rdregdata[0][i] != wrregdata[0][i])  
            {
                return FALSE;                 /* 配置结果未正确更新 */
            }      
        }
        for(i = 0; i < 6; i++)
        {
            if(rdregdata[1][i] != wrregdata[1][i])  
            {
                return FALSE;                 /* 配置结果未正确更新 */
            }      
        }
    }
    
    sLtc6803Blch = (bl_top << CELLNUM_BOTTOM) | bl_bottom;
    sLtc6803Status = TRUE;
    return TRUE;                              
    
}
/*
************************************************************************************************
*  Function:         Ltc6803Wr()
*  Description:      Ltc6803写命令
*  Calls:            Crc107();
*  Input:            INT8U cmd
*  Output:           无 
*  Return:           BOOLEAN
*  Others:           cmd为ltc6803的控制指令,cmd参数范围为STCVAD,STOWAD,STTMPAD;
*                    STCVAD, 电压检测;STOWAD, 断线检测;STTMPAD, 芯片内部温度采样                          
************************************************************************************************
*/
void Ltc6803Wr(INT8U cmd)
{   
    INT8U rddata = 0x00;                     /* 读取的数据值 */
    INT8U command = cmd;
    INT8U crc = 0;
    
    crc = Crc107(&command,1);                /* 获取控制指令的CRC值 */
    LTC6803_SS = SPI_ON;                     /* SPI通信打开 */   
    LTC6803_COMM(command,rddata);            /* 发送Ltc6803控制指令 */    
    LTC6803_COMM(crc,rddata);                /* 发送Ltc6803控制指令的CRC值 */
    
    if((cmd == STCVAD) || (cmd == STOWAD) || (cmd == STTMPAD) || (cmd == DAGN))    /* 单8位指令, 无后续数据 */
    {                      
        LTC6803_SS = SPI_OFF;                /* SPI通信关断 */  
    } 
}   
/*
************************************************************************************************
*  Function:         Ltc6803Rd()
*  Description:      Ltc6803电压读取函数
*  Calls:            Ltc6803Wr();Crc107();
*  Input:            INT16U *p_vcell
*  Output:           无
*  Return:           BOOLEAN
*  Others:           输入参数:p_vcell为电池电压存储数据的首地址                            
************************************************************************************************
*/
void Ltc6803Rd(INT16U *p_vcell)             
{    
    #if(CELLNUM <= 10)
        #define ACKNUM 16
    #else
        #define ACKNUM 19
    #endif   
    INT8U i, j;
    INT16U voltage = 0;            /* 初始化电压值 */              
    INT8U rdregdata[2][ACKNUM];       /* 电压接收数据缓冲区 */
    
    LTC6803_SS = SPI_ON;           /* SPI通信打开 */
    (void)Ltc6803Wr(RDCV);         /* 发起读电压命令 */
    for(i = 0; i < ACKNUM; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[0][i]);   /* 接收电压数据 */
    }
    for(i = 0; i < ACKNUM; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[1][i]);   /* 接收电压数据 */
    }
    LTC6803_SS = SPI_OFF;                   /* SPI通信关断 */
                                                                                               
    if((rdregdata[0][ACKNUM - 1] != Crc107(&rdregdata[0][0], ACKNUM - 1))
    || (rdregdata[1][ACKNUM - 1] != Crc107(&rdregdata[1][0], ACKNUM - 1)))    
    {        
        sLtc6803Status = FALSE;       /* CRC校验失败 */
       // return;                      
    }
    for(i = 0, j = 0; i < ((CELLNUM_BOTTOM + 1) >> 1); i++)             /* 电压数据提取，转换次数由CELLNUM确定 */ 
    { 
        if(j >= CELLNUM_BOTTOM)
        {
            break;
        }
        voltage        = 0;
        voltage        = rdregdata[0][i * 3];
        voltage       |= ((rdregdata[0][i * 3 + 1] & 0x0F) << 8); /* 12位数据的高四位在下一字节的低四位，低8位在该字节（1，4,7，10,13,16）*/
        voltage        = ((voltage - 512) * 3) >> 1;            /* 电压值 = (所得数据 - 512) * 1.5mV */
        *(p_vcell + j) = voltage;
        j++;
        if(j >= CELLNUM_BOTTOM)
        {
            break;
        } 
        voltage        = 0;
        voltage        = (rdregdata[0][i * 3 + 2] << 4);
        voltage       |= (((rdregdata[0][i * 3 + 1] & 0xF0) >> 4));/*12位数据的高八位在下一字节，低四位在该字节的高4位 （2，5,8，11,14,17) */
        voltage        = ((voltage - 512) * 3) >> 1;            /* 电压值 = (所得数据 - 512) * 1.5mV */
        *(p_vcell + j) = voltage; 
        j++;           
    }
    for(i = 0, j = CELLNUM_BOTTOM; i < ((CELLNUM_TOP + 1)>> 1); i++)             /* 电压数据提取，转换次数由CELLNUM确定 */ 
    {   
        if(j >= CELLNUM)
        {
            break;
        }
        voltage        = 0;
        voltage        = rdregdata[1][i * 3];
        voltage       |= ((rdregdata[1][i * 3 + 1] & 0x0F) << 8); /* 12位数据的高四位在下一字节的低四位，低8位在该字节（1，4,7，10,13,16）*/
        voltage        = ((voltage - 512) * 3) >> 1;            /* 电压值 = (所得数据 - 512) * 1.5mV */
        *(p_vcell + j) = voltage;
        j++;
        if(j >= CELLNUM)
        {
            break;
        } 
        voltage        = 0;
        voltage        = (rdregdata[1][i * 3 + 2] << 4);
        voltage       |= (((rdregdata[1][i * 3 + 1] & 0xF0) >> 4));/*12位数据的高八位在下一字节，低四位在该字节的高4位 （2，5,8，11,14,17) */
        voltage        = ((voltage - 512) * 3) >> 1;            /* 电压值 = (所得数据 - 512) * 1.5mV */
        *(p_vcell + j) = voltage; 
        j++;           
    }  
}
/*
************************************************************************************************
*  Function:         Ltc6803RdTemp()
*  Description:      Ltc6803Rd读内部温度命令
*  Calls:            Ltc6803Wr();Crc107();
*  Input:            无
*  Output:           无
*  Return:           BOOLEAN
*  Others:           读取芯片内部温度值(offset:-40℃)                  
************************************************************************************************
*/
INT8U Ltc6803RdTemp(void)
{
    return 0;
}
/*
************************************************************************************************
*  Function:         Crc107
*  Description:      CRC数据校验
*  Calls:            无
*  Input:            INT8U *p_buf, INT8U len
*  Output:           无 
*  Return:           INT8U
*  Others:           buff为p_buf数据首地址
*                    CRC校验,校验多项式为x^8+x^2+x^1+1,
*                    函数返回校验数据的计算所得CRC码
************************************************************************************************
*/ 
INT8U Crc107(INT8U *p_buf,INT8U len)
{
    INT8U crc = 0b01000001;         
    INT8U crc_half = 0;
    INT8U data_half = 0;
    
    while(len--)
    {
        data_half = *p_buf >> 4;
        crc_half = crc >> 4;   
        crc <<= 4;
        crc ^= Crc107_HalfList[crc_half ^ data_half];  
            
        data_half = *p_buf & 0x0F;
        crc_half = crc >> 4;        
        crc <<= 4;
        crc ^= Crc107_HalfList[crc_half ^ data_half];
        p_buf++;
    }  
    return crc;    
}
BOOLEAN Ltc6803Diag(void)
{
    INT8U i = 0;
    BOOLEAN status = FALSE;
    INT8U rdregdata[2][3];
    INT16U ref[2];
    BOOLEAN muxfail[2];
    (void)Ltc6803Wr(DAGN);
    LEDDATA = 1;    
    MCG_DelayMs(40);
    LEDDATA = 0;
    LTC6803_SS = SPI_ON;           /* SPI通信打开 */
    (void)Ltc6803Wr(RDDGNR);         /* 发起读电压命令 */
    
    for(i = 0; i < 3; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[0][i]);   /* 接收BOTTOM电压数据 */
    } 
    for(i = 0; i < 3; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[1][i]);   /* 接收TOP电压数据 */
    } 
    LTC6803_SS = SPI_OFF;           /* SPI通信关闭 */   
    if((rdregdata[0][2] == Crc107(&rdregdata[0][0], 2))
    && (rdregdata[1][2] == Crc107(&rdregdata[1][0], 2)))    
    {                          
        ref[0] = rdregdata[0][1];
        muxfail[0] = (ref[0] & 0x20) >> 5;  
        ref[0] = ((ref[0] & 0x0F) << 8) | rdregdata[0][0];
        ref[0] = ((ref[0] - 512) * 3) >> 1;      /* BOTTOM诊断参考电压 */
        
        ref[1] = rdregdata[1][1];
        muxfail[1] = (ref[1] & 0x20) >> 5;  
        ref[1] = ((ref[1] & 0x0F) << 8) | rdregdata[1][0];
        ref[1] = ((ref[1] - 512) * 3) >> 1;      /* TOP诊断参考电压 */
        
        if((muxfail == FALSE) && (muxfail == FALSE))
        {
            if((ref[0] >= 2100) && (ref[0] <= 2900) && (ref[1] >= 2100) && (ref[1] <= 2900)) 
            {
               status = TRUE;    
            }
        }
    }
    return status;
}