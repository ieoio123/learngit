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
*Descriprion :   ��ƬLtc6803����ز��� 
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "Ltc6803.h"
/************************************const value define***************************************/
const INT8U Crc107_HalfList[]=                   /* Crc107���ֽڲ������ */
{
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
    0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D
};
/************************************Variable define******************************************/
static BOOLEAN sLtc6803Status = FALSE;           /* Ltc6803�豸״̬ */
static INT32U  sLtc6803Blch = 0;                 /* Ltc6803����״̬ */
/*
************************************************************************************************
*  Function:         Ltc6803Devstatus()
*  Description:      ��ȡLct6803�ĵ�ǰ�豸״̬
*  Calls:            
*  Input:           
*  Output:           �� 
*  Return:           BOOLEAN
*  Others:           ����Ltc6803���豸״̬,
*                    TRUE--�豸����,FALSE--�豸�����쳣        
************************************************************************************************
*/
BOOLEAN Ltc6803Devstatus(void)
{
    return sLtc6803Status;
}
/*
************************************************************************************************
*  Function:         Ltc6803CfgInit()
*  Description:      Lct6803�����üĴ�����ʼ��
*  Calls:            Ltc6803Cfg();
*  Input:            INT32U blch
*  Output:           �� 
*  Return:           BOOLEAN
*  Others:           ����Ltc6803���豸״̬,
*                    TRUE--�豸����,FALSE--�豸�����쳣        
************************************************************************************************
*/
BOOLEAN Ltc6803CfgInit(INT32U blch)
{
    sLtc6803Status = Ltc6803Cfg(blch);                /* ����Lct6803�Ĵ���   */
    return sLtc6803Status;
}
/*
************************************************************************************************
*  Function:         Ltc6803CellVolDetect()
*  Description:      Lct6803�ĵ�ѹ��������
*  Calls:            Ltc6803Wr();MCG_DelayMs();Ltc6803Rd()
*  Input:            INT16U *p_vcell
*  Output:           �� 
*  Return:           BOOLEAN
*  Others:           �������p_vcellΪ��ѹ�洢�����׵�ַ,
*                    ����Ltc6803���豸״̬,TRUE--�豸����,FALSE--�豸�����쳣        
************************************************************************************************
*/
BOOLEAN Ltc6803CellVolDetect(INT16U *p_vcell)
{
    INT32U blch = sLtc6803Blch;     
    
    sLtc6803Status = Ltc6803Cfg(0);                   /* ����Lct6802�Ĵ���    */
    Ltc6803Wr(STCVAD);                                /* ����12�������������� */
    MCG_DelayMs(15);                                  /* �ȴ��������         */
    Ltc6803Rd(p_vcell);                               /* ��ȡ������ѹֵ       */
    sLtc6803Status = Ltc6803Cfg(blch);                /* ����Lct6802�Ĵ���    */
    return sLtc6803Status;
}
/*
************************************************************************************************
*  Function:         Ltc6803OpenDetect()
*  Description:      ��ز����Ķ��߼��
*  Calls:            Ltc6803Wr();MCG_DelayMs();Ltc6803Rd()
*  Input:            
*  Output:           �� 
*  Return:           INT8U
*  Others:           ���ؼ��״̬      
************************************************************************************************
*/
BOOLEAN Ltc6803OpenDetect(void)
{    
    INT8U i;
    INT16S openvcell[CELLNUM];                        /* ���߼���ѹ���� */
    
    (void)Ltc6803Wr(STOWAD);                          /* ����12�ڿ��߲������� */
    MCG_DelayMs(15);                                  /* �ȴ��������         */
    Ltc6803Rd((INT16U *)openvcell);                             /* ��ȡ������ѹֵ       */
     
    if(sLtc6803Status == FALSE)
    {
        return FALSE;
    }
    for(i = 0; i < CELLNUM; i++)
    {
        if(openvcell[i] < OPEN_VOL_DIFF)              /* ��ѹֵС�ڶ�����ֵ   */
        {
            return FALSE;                             
        }
    }                                                 
    return TRUE;                                      /* ����δ�������� */
}
/*
************************************************************************************************
*  Function:         Ltc6803TempDectect()
*  Description:      ��ز����Ķ��߼��
*  Calls:            Ltc6803Wr();MCG_DelayMs();Ltc6803Rd()
*  Input:            INT8U *p_temp
*  Output:           �� 
*  Return:           INT8U
*  Others:           �������p_tempΪ�¶ȴ洢�����׵�ַ     
************************************************************************************************
*/
void Ltc6803TempDectect(INT8U *p_temp)
{
    (void)Ltc6803Wr(STTMPAD);                         /* ����оƬ�ڲ��¶Ȳ������� */
    MCG_DelayMs(10);                                  /* �ȴ��������             */
    *p_temp = Ltc6803RdTemp();                        /* ��ȡ�����¶Ƚ��         */
}
/*
************************************************************************************************
*  Function:         Ltc6803Cfg()
*  Description:      ���üĴ���,����豸״̬
*  Calls:            Ltc6803Wr();Crc107();
*  Input:            INT32U blchannel
*  Output:           �� 
*  Return:           BOOLEAN
*  Others:           blchannelоƬ�������ͨ��ֵ��
*                    ����Ltc6803���豸״̬,TRUE--�豸����,FALSE--�豸�����쳣                        
************************************************************************************************
*/
BOOLEAN Ltc6803Cfg(INT32U blchannel) 
{                      
    INT8U i;                               
    INT8U wrregdata[2][7];                                 /* Ltc6803�Ĵ�������д����*/
    INT8U rdregdata[2][7];                                 /* Ltc6803�Ĵ������ö�����*/
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
    wrregdata[0][6]  = Crc107(&wrregdata[0][0],6);/* ��ȡд�Ĵ�����Ϣ��CRC�� */ 
    /* Bottom Registers */
    wrregdata[1][0]  = 0x91;                      /* enable watchdog,level polling,12-cell,standby mode */ 
    wrregdata[1][1]  = blch_l_bottom;             /* Bleed Channel: 8 ~ 1                               */
    wrregdata[1][2]  = blch_h_bottom | mask_ir_l; /* Mask Cell Interrupts: 4 ~ 1, Bleed Channel: 12 ~ 9 */   
    wrregdata[1][3]  = mask_ir_h;                 /* Mask Cell Interrupts: 12 ~ 5                       */ 
    wrregdata[1][4]  = 0x49;                      /* VUV=0x2A*16*1.5mV=1.008V                           */
    wrregdata[1][5]  = 0xE0;                      /* VOV=0xD0*16*1.5mV=4.992V                           */ 
    wrregdata[1][6]  = Crc107(&wrregdata[1][0],6);/* ��ȡд�Ĵ�����Ϣ��CRC�� */ 
    
    (void)Ltc6803Wr(WRCFG);    
    LTC6803_SS = SPI_ON;                      /* SPIͨ�Ŵ� */                              
    for(i = 0; i < 7;i ++)
    {
        LTC6803_COMM(wrregdata[0][i], rddata);   /* �������üĴ�����Ϣ */
    }                          
    for(i = 0; i < 7;i ++)
    {
        LTC6803_COMM(wrregdata[1][i], rddata);   /* �������üĴ�����Ϣ */
    }
    LTC6803_SS = SPI_OFF;                     /* SPIͨ�Źر� */ 
    
    LTC6803_SS = SPI_ON;                      /* SPIͨ�Ŵ� */                            
    (void)Ltc6803Wr(RDCFG);
    for(i=0;i<7;i++)
    {
        LTC6803_COMM(0x00,rdregdata[1][i]);      /* ��ȡ���üĴ�����Ϣ */
    }
    for(i=0;i<7;i++)
    {
        LTC6803_COMM(0x00,rdregdata[0][i]);      /* ��ȡ���üĴ�����Ϣ */
    }
    LTC6803_SS = SPI_OFF;                     /* SPIͨ�Źر� */ 
    
    if((rdregdata[0][6] != Crc107(&rdregdata[0][0],6)) 
    || (rdregdata[1][6] != Crc107(&rdregdata[1][0],6))) 
    {        
        //return FALSE;                         /* CRCУ��ʧ�� */
    }else
    {    
        for(i = 0; i < 6; i++)
        {
            if(rdregdata[0][i] != wrregdata[0][i])  
            {
                return FALSE;                 /* ���ý��δ��ȷ���� */
            }      
        }
        for(i = 0; i < 6; i++)
        {
            if(rdregdata[1][i] != wrregdata[1][i])  
            {
                return FALSE;                 /* ���ý��δ��ȷ���� */
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
*  Description:      Ltc6803д����
*  Calls:            Crc107();
*  Input:            INT8U cmd
*  Output:           �� 
*  Return:           BOOLEAN
*  Others:           cmdΪltc6803�Ŀ���ָ��,cmd������ΧΪSTCVAD,STOWAD,STTMPAD;
*                    STCVAD, ��ѹ���;STOWAD, ���߼��;STTMPAD, оƬ�ڲ��¶Ȳ���                          
************************************************************************************************
*/
void Ltc6803Wr(INT8U cmd)
{   
    INT8U rddata = 0x00;                     /* ��ȡ������ֵ */
    INT8U command = cmd;
    INT8U crc = 0;
    
    crc = Crc107(&command,1);                /* ��ȡ����ָ���CRCֵ */
    LTC6803_SS = SPI_ON;                     /* SPIͨ�Ŵ� */   
    LTC6803_COMM(command,rddata);            /* ����Ltc6803����ָ�� */    
    LTC6803_COMM(crc,rddata);                /* ����Ltc6803����ָ���CRCֵ */
    
    if((cmd == STCVAD) || (cmd == STOWAD) || (cmd == STTMPAD) || (cmd == DAGN))    /* ��8λָ��, �޺������� */
    {                      
        LTC6803_SS = SPI_OFF;                /* SPIͨ�Źض� */  
    } 
}   
/*
************************************************************************************************
*  Function:         Ltc6803Rd()
*  Description:      Ltc6803��ѹ��ȡ����
*  Calls:            Ltc6803Wr();Crc107();
*  Input:            INT16U *p_vcell
*  Output:           ��
*  Return:           BOOLEAN
*  Others:           �������:p_vcellΪ��ص�ѹ�洢���ݵ��׵�ַ                            
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
    INT16U voltage = 0;            /* ��ʼ����ѹֵ */              
    INT8U rdregdata[2][ACKNUM];       /* ��ѹ�������ݻ����� */
    
    LTC6803_SS = SPI_ON;           /* SPIͨ�Ŵ� */
    (void)Ltc6803Wr(RDCV);         /* �������ѹ���� */
    for(i = 0; i < ACKNUM; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[0][i]);   /* ���յ�ѹ���� */
    }
    for(i = 0; i < ACKNUM; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[1][i]);   /* ���յ�ѹ���� */
    }
    LTC6803_SS = SPI_OFF;                   /* SPIͨ�Źض� */
                                                                                               
    if((rdregdata[0][ACKNUM - 1] != Crc107(&rdregdata[0][0], ACKNUM - 1))
    || (rdregdata[1][ACKNUM - 1] != Crc107(&rdregdata[1][0], ACKNUM - 1)))    
    {        
        sLtc6803Status = FALSE;       /* CRCУ��ʧ�� */
       // return;                      
    }
    for(i = 0, j = 0; i < ((CELLNUM_BOTTOM + 1) >> 1); i++)             /* ��ѹ������ȡ��ת��������CELLNUMȷ�� */ 
    { 
        if(j >= CELLNUM_BOTTOM)
        {
            break;
        }
        voltage        = 0;
        voltage        = rdregdata[0][i * 3];
        voltage       |= ((rdregdata[0][i * 3 + 1] & 0x0F) << 8); /* 12λ���ݵĸ���λ����һ�ֽڵĵ���λ����8λ�ڸ��ֽڣ�1��4,7��10,13,16��*/
        voltage        = ((voltage - 512) * 3) >> 1;            /* ��ѹֵ = (�������� - 512) * 1.5mV */
        *(p_vcell + j) = voltage;
        j++;
        if(j >= CELLNUM_BOTTOM)
        {
            break;
        } 
        voltage        = 0;
        voltage        = (rdregdata[0][i * 3 + 2] << 4);
        voltage       |= (((rdregdata[0][i * 3 + 1] & 0xF0) >> 4));/*12λ���ݵĸ߰�λ����һ�ֽڣ�����λ�ڸ��ֽڵĸ�4λ ��2��5,8��11,14,17) */
        voltage        = ((voltage - 512) * 3) >> 1;            /* ��ѹֵ = (�������� - 512) * 1.5mV */
        *(p_vcell + j) = voltage; 
        j++;           
    }
    for(i = 0, j = CELLNUM_BOTTOM; i < ((CELLNUM_TOP + 1)>> 1); i++)             /* ��ѹ������ȡ��ת��������CELLNUMȷ�� */ 
    {   
        if(j >= CELLNUM)
        {
            break;
        }
        voltage        = 0;
        voltage        = rdregdata[1][i * 3];
        voltage       |= ((rdregdata[1][i * 3 + 1] & 0x0F) << 8); /* 12λ���ݵĸ���λ����һ�ֽڵĵ���λ����8λ�ڸ��ֽڣ�1��4,7��10,13,16��*/
        voltage        = ((voltage - 512) * 3) >> 1;            /* ��ѹֵ = (�������� - 512) * 1.5mV */
        *(p_vcell + j) = voltage;
        j++;
        if(j >= CELLNUM)
        {
            break;
        } 
        voltage        = 0;
        voltage        = (rdregdata[1][i * 3 + 2] << 4);
        voltage       |= (((rdregdata[1][i * 3 + 1] & 0xF0) >> 4));/*12λ���ݵĸ߰�λ����һ�ֽڣ�����λ�ڸ��ֽڵĸ�4λ ��2��5,8��11,14,17) */
        voltage        = ((voltage - 512) * 3) >> 1;            /* ��ѹֵ = (�������� - 512) * 1.5mV */
        *(p_vcell + j) = voltage; 
        j++;           
    }  
}
/*
************************************************************************************************
*  Function:         Ltc6803RdTemp()
*  Description:      Ltc6803Rd���ڲ��¶�����
*  Calls:            Ltc6803Wr();Crc107();
*  Input:            ��
*  Output:           ��
*  Return:           BOOLEAN
*  Others:           ��ȡоƬ�ڲ��¶�ֵ(offset:-40��)                  
************************************************************************************************
*/
INT8U Ltc6803RdTemp(void)
{
    return 0;
}
/*
************************************************************************************************
*  Function:         Crc107
*  Description:      CRC����У��
*  Calls:            ��
*  Input:            INT8U *p_buf, INT8U len
*  Output:           �� 
*  Return:           INT8U
*  Others:           buffΪp_buf�����׵�ַ
*                    CRCУ��,У�����ʽΪx^8+x^2+x^1+1,
*                    ��������У�����ݵļ�������CRC��
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
    LTC6803_SS = SPI_ON;           /* SPIͨ�Ŵ� */
    (void)Ltc6803Wr(RDDGNR);         /* �������ѹ���� */
    
    for(i = 0; i < 3; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[0][i]);   /* ����BOTTOM��ѹ���� */
    } 
    for(i = 0; i < 3; i++) 
    {
        LTC6803_COMM(0xFF, rdregdata[1][i]);   /* ����TOP��ѹ���� */
    } 
    LTC6803_SS = SPI_OFF;           /* SPIͨ�Źر� */   
    if((rdregdata[0][2] == Crc107(&rdregdata[0][0], 2))
    && (rdregdata[1][2] == Crc107(&rdregdata[1][0], 2)))    
    {                          
        ref[0] = rdregdata[0][1];
        muxfail[0] = (ref[0] & 0x20) >> 5;  
        ref[0] = ((ref[0] & 0x0F) << 8) | rdregdata[0][0];
        ref[0] = ((ref[0] - 512) * 3) >> 1;      /* BOTTOM��ϲο���ѹ */
        
        ref[1] = rdregdata[1][1];
        muxfail[1] = (ref[1] & 0x20) >> 5;  
        ref[1] = ((ref[1] & 0x0F) << 8) | rdregdata[1][0];
        ref[1] = ((ref[1] - 512) * 3) >> 1;      /* TOP��ϲο���ѹ */
        
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