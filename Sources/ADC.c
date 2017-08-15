/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     ADC.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   ������MC9S12G128��Ƭ��ƽ̨
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "ADC.h"
#include "TYPEDEFINE.h"
#include "CONFIG.h"
/************************************const value define***************************************/
/* NTC�����ߵ���ֵ--�¶ȱ� */
const INT16U ResTemp[161] = 
{                                                                                                                   
    29686,27929,26277,24726,23270,21958,20664,19453,18321,17261,                                         /* -40~-31 */
    16268,15348,14486,13677,12916,12202,11536,10909,10319,9764,                                          /* -30~-21 */
    9241,8777,8338,7922,7527,7152,6765,6400,6060,5737,5434,5153,4888,4638,4402,4180,3970,3772,3585,3409, /* -20~-1  */
    3242,3083,2933,2790,2656,2529,2409,2296,2188,2086,1990,1895,1806,1722,1642,1567,1496,1429,1365,1305, /*   0~19  */
    1247,1193,1141,1092,1045,1000,958,917,879,842,807,773,742,711,682,654,628,602,578,555,               /*  20~39  */
    533,512,491,472,453,436,419,403,388,373,359,345,333,320,308,297,296,275,265,255,                     /*  40~59  */
    246,237,229,222,214,207,200,193,187,180,174,168,163,157,152,147,142,138,133,129,                     /*  60~79  */
    125,121,117,113,110,106,103,100,97,94,91,88,85,83,80,78,76,73,71,69,                                 /*  80~99  */
    67,65,63,62,60,58,57,55,53,52,50,49,48,46,45,44,43,41,40,39,37                                       /* 100~120 */
};
/* LECU����NTC����ֵ--�¶ȱ� */
const INT16U BDResTemp[191] =
{  
    26383,24745,23221,21802,20482,19251,18104,17033,16034,15101,     /* -40~-31 */
    14230,13415,12653,11940,11273,10647,10061,9512,8997,8513,        /* -30~-21 */
    8059,7632,7231,6854,6499,6165,5851,5555,5276,5013,               /* -20~-11 */
    4765,4531,4310,4102,3905,3719,3543,3376,3219,3069,               /* -10~-1  */
    2928,2795,2668,2548,2434,2326,2223,2126,2034,1946,               /*   0~9   */
    1862,1783,1708,1636,1568,1503,1441,1382,1326,1273,               /*  10~19  */ 
    1222,1173,1127,1083,1040,1000,962,925,890,856,                   /*  20~29  */
    824,793,764,736,709,683,659,635,612,591,                         /*  30~39  */
    570,550,531,513,495,478,462,446,431,417,                         /*  40~49  */
    403,390,377,365,353,342,331,321,310,301,                         /*  50~59  */
    291,282,274,265,257,249,242,235,228,221,                         /*  60~69  */
    215,208,202,196,191,185,180,175,170,165,                         /*  70~79  */
    161,156,152,148,144,140,136,133,129,126,                         /*  80~89  */
    122,119,116,113,110,107,105,102,99,97,                           /*  90~99  */
    94,92,90,88,86,83,81,79,78,76,                                   /* 100~109 */
    74,72,71,69,67,66,64,63,61,60,                                   /* 110~119 */
    59,57,56,55,54,52,51,50,49,48,                                   /* 120~129 */
    47,46,45,44,43,42,41,41,40,39,                                   /* 130~139 */
    38,37,37,36,35,34,34,33,32,32,31                                 /* 140~150 */                                                                 
};
/************************************Variable define******************************************/
static INT8U sAdTemp[NUMTEMP][FILTER_NUM];                           /* �¶����ݻ����� */
/*
************************************************************************************************
*  Function:         ADCInit()
*  Description:      ADC��ʼ��
*  Calls:            ��
*  Input:            ��
*  Output:           �� 
*  Return:           ��
*  Others:           ��         
************************************************************************************************
*/
void ADC_Init(void)
{
    ATDCTL1     = 0b00111111;
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ |bit3:0,External Trigger Channel Select(select AN11);   
	                  |||||_____/ 
	                  ||||______  bit4, The internal sample capacitor is discharged before sampling the channel;
	                  |||_______
	                  ||________> bit6:5, 10-bit A/D conversion results;
	                  |_________  bit7, select the external trigger source to AN0~AN11; 
    */
    ATDCTL3     = 0b10001011;
    /*              ||||||||__
	                  |||||||___> bit1:0, ATD Freezes immediatey in Freeze Mode(Breakpoint);  
	                  ||||||____  bit2, Conversion results are placed in the sequence up to the sequence length; 
	                  |||||_____  
	                  ||||______\  
	                  |||_______ |bit6:3, set the number of conversion per sequence is 1(inital);
	                  ||________/
	                  |__________ bit7, Right justified data in the result registers; 
    */    
    ATDCTL4     = 0b01100100;
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ | 
	                  |||||_____ |bit4:0, set Fatdclk = Fbus / (2 * (0b00100 + 1)) = 800KHz; 
	                  ||||______/  
	                  |||_______ 
	                  ||________\
	                  |_________/ bit7:5, ATD sample time is 4 units of ATD Clock Cycles,Tad = 4 * 1 /Fatdclk = 5us;   
    */
    ATDCTL5     = 0b00000000;
    /*              ||||||||__
	                  |||||||___\
	                  ||||||____ |bit3:0, the channel to be examined is AN0(inital);  
	                  |||||_____/ 
	                  ||||______  bit4, Samled only one channel;
	                  |||_______  bit5, Single conversion sequence;
	                  ||________  bit6, Special channel conversions disabled;
	                  |_________
    */
} 
/*
************************************************************************************************
*  Function:         AdcConvert()
*  Description:      ADCת��
*  Calls:            ��
*  Input:            INT8U ch
*  Output:           �� 
*  Return:           INT16U
*  Others:           �������:chΪAD������ͨ��ֵ, 
*                    ��������ADת���Ľ��         
************************************************************************************************
*/ 
INT16U AdcConvert(INT8U ch)
{   
    ATDSTAT0 &= 0xF0;                                          /* clear sequence complete flag */
    ATDCTL5   = ch;                                            /* set the channel to be examined */   

    while(!ATDSTAT0_SCF);                                      /* wait sequence complete */    
    
    return ATDDR0;                                             /* store the conersion results */   
}
/*
************************************************************************************************
*  Function:         AdcTemp()
*  Description:      �¶Ȳ�������
*  Calls:            ��
*  Input:            const INT8U *p_ch,INT8U *p_temp
*  Output:           �� 
*  Return:           �� 
*  Others:           �������:p_chΪAD������ͨ��ֵ����׵�ַ,p_tempΪ�¶ȴ洢�����׵�ַ          
************************************************************************************************
*/ 
void AdcTemp(const INT8U *p_ch,INT8U *p_temp)
{
    INT8U      i, n;
    INT16U     result = 0; 
    INT16U     res = 0;                                        
    INT16U     sum = 0;
    static BOOLEAN sAdcTemp_first_rap = TRUE;                  /* first rap���� */
    static INT8U   sAdcTemp_temp_location = 0;                 /* range of 0~FILTER_NUM-1*/
    
    for(i = 0; i < NUMTEMP; i++)
    {
        n = 0;
        result = AdcConvert(*(p_ch + i));         
        if(i <= NUMTEMP)                                       /* NTC������ */
        {
            res = (INT32U)(1000) * result / (1024 - result);   /* ������ֵ */
            
            if(res > ResTemp[0])
            {
                n = 0;
            }else if(res < ResTemp[160])
            {
                n = 160;
            }else
            {
                while((res < ResTemp[n]) && (n < 160))         /* ����ֵ����ֵ-�¶ȱ�, ����ƫ��-40�� */
                {
                    n++;                                           
                }
                if((res - ResTemp[n]) > (ResTemp[n-1] - res))  /* �������� */
                {
                	  n--;
                }
            }
        }else                                                  /* ����NTC */
        {
            res = (INT32U)(100) * result / (1024 - result);    /* ������ֵ */
            
            if(res > BDResTemp[0])
            {
                n = 0;
            }else if(res < BDResTemp[190])
            {
                n = 160;
            }else
            {
                while((res < BDResTemp[n]) && (n < 190))                 /* ����ֵ����ֵ-�¶ȱ�, ����ƫ��-40�� */
                {
                    n++;                                           
                }
                if((res - BDResTemp[n]) > (BDResTemp[n-1] - res))          /* �������� */
                {
                	  n--;
                }
            }
        }/* end of if(i != NUMTEMP-1) */
        sAdTemp[i][sAdcTemp_temp_location] = n;
    }
    sAdcTemp_temp_location++;
    if(sAdcTemp_temp_location == FILTER_NUM)                       /* ��һȦ��� */
    {
        sAdcTemp_temp_location = 0;
        sAdcTemp_first_rap = FALSE;
    }
    if(sAdcTemp_first_rap == FALSE)
    {
        for(i = 0; i < NUMTEMP; i++)
        {        
            p_temp[i] = AdcFilter(&sAdTemp[i][0]);                 /* �¶������˲� */   
        }
    }
    else
    {
        for(i = 0; i < NUMTEMP; i++)
        {       
            for(sum = 0, n = 0; n < sAdcTemp_temp_location; n++)
            {
                sum += sAdTemp[i][n];
            }
            p_temp[i] = sum / sAdcTemp_temp_location;         /* ��һ��,ֱ��ȡƽ��ֵ */
        }         
    }                  
}
/*
************************************************************************************************
*  Function:         AdcFilter()
*  Description:      AD�˲�����
*  Calls:            ��
*  Input:            INT8U *p_value
*  Output:           �� 
*  Return:           INT8U
*  Others:           �������:p_valueΪ�˲������׵�ַ,��ȥ�����Сֵ,ȡƽ��ֵ(FILTER_NUM>2)
*                    ���������˲���������ֵ         
************************************************************************************************
*/ 
INT8U AdcFilter(INT8U *p_value)
{
    INT8U max = *(p_value + 0);                               /* ��ʼ���������ֵ */
    INT8U min = *(p_value + 0);                               /* ��ʼ��������Сֵ */
    INT8U   i  =             0;             
    INT16U sum =             0;
    INT8U avr =             0;
    
    for(i = 0; i < FILTER_NUM; i++)
    {
        if(*(p_value + i) > max)
        {
            max = *(p_value + i);                              /* ���������ֵ */
        }else if(*(p_value + i) < min)
        {
            min = *(p_value + i);                              /* ��������Сֵ */
        }
        sum += *(p_value + i);                                 /* ���������ܺ� */
    }
    #if((FILTER_NUM > 2))
        sum = sum - max - min;                                 /* ��ȥ�����Сֵ,ȡƽ��ֵ */
        avr = sum / (FILTER_NUM - 2);                                         
    #else
        avr = sum / FILTER_NUM;                                /* ȡƽ��ֵ */                                      
    #endif
    
    return avr;                                                 
}