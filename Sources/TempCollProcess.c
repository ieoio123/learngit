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
*Descriprion :   适用于MC9S12G128单片机平台
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "TempCollProcess.h"
#include "MixDriver.h"
#include "CONFIG.h"
/************************************const value define***************************************/
/* NTC采样线的阻值--温度表 */
const INT16U ResTemp[RESTEMP_SIZE] = 
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
/* LECU板上NTC的阻值--温度表 */
const INT16U PcbResTemp[PCBRESTEMP_SIZE] =
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
/* 温度采样AD通道表 */ 
const INT8U ResTempCh[NUMTEMP] = 
{
    ADCCH0,            
    ADCCH1,          
    ADCCH2,                      
};
const INT8U PcbResTempCh = ADCCH_PCB; 
/************************************Variable define******************************************/
static INT8U sTempBuf[NUMTEMP][FILTER_NUM];                           /* 温度数据缓冲区 */
static INT8U sPcbTempBuf[FILTER_NUM];                                  /* 板上温度数据缓冲区 */
INT16U TempAd[NUMTEMP];                                               /* 温度采样AD值 */
INT16U PcbTempAd;                                                      /* 板上温度采样AD值 */
INT8U ResTemp_LookUp(INT8U value, INT8U *p_tab,INT8U size)
{
    INT8U index = 0;
    INT8U n = 0;
    if(value >= *p_tab)
    {
        index = 0;
    }else
    {
        while((value < *(p_tab + index)) && (n < size - 1))    /* 由阻值查阻值-温度表, 表内偏移-40℃ */
        {
            n++;                                           
        }
        if((value - *(p_tab + n)) > (*(p_tab + n - 1) - value))  /* 四舍五入 */
        {
        	  n--;
        }
    } 
    return index;   
}
/*
************************************************************************************************
*  Function:         AdcTemp()
*  Description:      温度采样函数
*  Calls:            无
*  Input:            const INT8U *p_ch,INT8U *p_temp
*  Output:           无 
*  Return:           无 
*  Others:           输入参数:p_ch为AD采样的通道值组的首地址,p_temp为温度存储数组首地址          
************************************************************************************************
*/ 
void ResTemp_Samp(INT8U *p_temp)
{
    INT8U      i, n;
    INT16U     result = 0; 
    INT16U     res = 0;                                        
    INT16U     sum = 0;
    static BOOLEAN sAdcTemp_first_rap = TRUE;                  /* first rap采样 */
    static INT8U   sAdcTemp_temp_location = 0;                 /* range of 0~FILTER_NUM-1*/
    
    for(i = 0; i < NUMTEMP; i++)
    {
        result = ADC_Convert(ResTempCh[i]); 
        res = (INT32U)(SERIE_SRES) * result / (1024 - result);   /* 计算阻值, 10位AD值*/          
        sTempBuf[i][sAdcTemp_temp_location] = ResTemp_LookUp(res, &ResTemp[0], RESTEMP_SIZE);
        TempAd[i] = result;
    }   
    {
        result = ADC_Convert(PcbResTempCh); 
        res = (INT32U)(PCB_SERIE_SRES) * result / (1024 - result);    /* 计算阻值, 10位AD值 */    
        sPcbTempBuf[sAdcTemp_temp_location] = ResTemp_LookUp(res, &PcbResTemp[0], PCBRESTEMP_SIZE);
        PcbTempAd = result;
    }
    
    sAdcTemp_temp_location++;
    if(sAdcTemp_temp_location >= FILTER_NUM)                       /* 第一圈完成 */
    {
        sAdcTemp_temp_location = 0;
        sAdcTemp_first_rap = FALSE;
    }
    if(sAdcTemp_first_rap == FALSE)
    {
        for(i = 0; i < NUMTEMP; i++)
        {        
            *(p_temp + i) = ResTemp_Filter(&sTempBuf[i][0]);           /* 温度数据滤波 */   
        }
        *(p_temp + NUMTEMP) = ResTemp_Filter(&sPcbTempBuf[0]);          /* 温度数据滤波 */
    }else
    {                                                             /* 第一圈, 直接赋值*/
        for(i = 0; i < NUMTEMP; i++)
        {       
            *(p_temp + i) = sTempBuf[i][0];         
        }
        *(p_temp + NUMTEMP) = sPcbTempBuf[0];           
    }                  
}
/*
************************************************************************************************
*  Function:         AdcFilter()
*  Description:      AD滤波函数
*  Calls:            无
*  Input:            INT8U *p_value
*  Output:           无 
*  Return:           INT8U
*  Others:           输入参数:p_value为滤波数组首地址,舍去最大最小值,取平均值(FILTER_NUM>2)
*                    函数返回滤波处理后的数值         
************************************************************************************************
*/ 
INT8U ResTemp_Filter(INT8U *p_value)
{
    INT8U max = *(p_value + 0);                               /* 初始化数组最大值 */
    INT8U min = *(p_value + 0);                               /* 初始化数组最小值 */
    INT8U   i  =             0;             
    INT16U sum =             0;
    INT8U avr =             0;
    
    for(i = 0; i < FILTER_NUM; i++)
    {
        if(*(p_value + i) > max)
        {
            max = *(p_value + i);                              /* 找数组最大值 */
        }else if(*(p_value + i) < min)
        {
            min = *(p_value + i);                              /* 找数组最小值 */
        }
        sum += *(p_value + i);                                 /* 计算数组总和 */
    }
    #if((FILTER_NUM > 2))
        sum = sum - max - min;                                 /* 舍去最大最小值,取平均值 */
        avr = sum / (FILTER_NUM - 2);                                         
    #else
        avr = sum / FILTER_NUM;                                /* 取平均值 */                                      
    #endif
    
    return avr;                                                 
}