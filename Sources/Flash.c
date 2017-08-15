/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     Flash.c
*Programmer(s) : Zeng Qunxin
*Descriprion :   ������MC9S12G128��Ƭ��ƽ̨
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0          Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#include "Flash.h"
/*
************************************************************************************************
*  Function:         EE_Command()
*  Description:      ����Flashָ��
*  Calls:            ��
*  Input:            INT8U params, INT8U command, INT8U ccob0, 
*                    INT16U ccob1, INT16U ccob2, INT16U ccob3, INT16U ccob4, INT16U ccob5
*  Output:           ��
*  Return:           INT8U
*  Others:           ��������Flashָ�����            
************************************************************************************************
*/
INT8U EE_Command(INT8U params, INT8U command, INT8U ccob0, INT16U ccob1, INT16U ccob2, INT16U ccob3, INT16U ccob4, INT16U ccob5)
{   
    if(!FCLKDIV_FDIVLD) 
    {
        return 0;                                        /* FCLKDIV register has not been written since the last reset */
    }
    if(FSTAT_CCIF == 1)	                                 /* Flash command has completed */
    {																	
		    FSTAT = (FSTAT_FPVIOL_MASK | FSTAT_ACCERR_MASK | FSTAT_MGSTAT_MASK); /* Clear Flash protection violation flag and Flash access error flag and Flash error during execution*/

        FCCOBIX = 0;                                     /* Ready for Writing the command id(ccob0) */ 

        FCCOBHI = command;                               /* Write Flash Command */
        
    		FCCOBLO = ccob0;                                 /* Write global address[17:16] */
                                                
        if(++FCCOBIX != params) 
        {
            FCCOB = ccob1; 		                           /* Write next data word to CCOB buffer,write global address[15:0](Typical Usage) */
            if(++FCCOBIX != params) 
            {
  		          FCCOB = ccob2; 			                     /* Write next data word to CCOB buffer */ 
                if(++FCCOBIX != params) 
                {
   		              FCCOB = ccob3; 		                   /* Write next data word to CCOB buffer */ 
                    if(++FCCOBIX != params) 
                    {
                        FCCOB = ccob4; 			             /* Write next data word to CCOB buffer */
                        if(++FCCOBIX != params) 
         		            {
         		                FCCOB = ccob5; 			         /* Write next data word to CCOB buffer */ 
                        }
                    } 											
                }  
	          }
        } /* end of if(++FCCOBIX != params) */
        FCCOBIX = params - 1;                            /* Set Common Command Register Index */
       
        FSTAT = 0x80;                                    /* Clear command buffer empty flag by writing a 1 to it and start the command */		
        while (!FSTAT_CCIF); 			                       /* Wait for the command to complete */
        
        return(FSTAT&0x7F);						                   /* state of flash command */
    } /* end of if(FSTAT_CCIF == 1) */
	  return(0x80);							                           /* state machine busy */
}
/*
************************************************************************************************
*  Function:         EE_DwordErase()
*  Description:      EEPROM˫�ֲ�������
*  Calls:            EE_Command
*  Input:            INT16U addr
*  Output:           ��
*  Return:           INT8U
*  Others:           �������:addrΪ��������ʼ��ַ(0x00400 ~ 0x013FF), 4byte/sector 
*                    �������ز������          
************************************************************************************************
*/
INT8U EE_DwordErase(INT16U addr) 
{      
    INT8U  Hi_add;
    INT16U Lo_add;
    INT8U ret_value; 
        
    Hi_add = 0x00;
    Lo_add = addr;
    
    if((addr >= 0x0400) && (addr <= 0x13ff))               /* ����Ч��ַ��Χ�� */
    {
      ret_value = EE_Command(2, ERASE_D_FLASH_SECTOR, Hi_add, Lo_add,0,0,0,0);  /* ����ָ�� */
    }
    return (ret_value);
}
/*
************************************************************************************************
*  Function:         EE_WordWrite()
*  Description:      EEPROM���ֱ�̺���
*  Calls:            EE_Command
*  Input:            INT16U addr, INT16U data
*  Output:           ��
*  Return:           INT8U
*  Others:           �������:addrΪ��̵���ʼ��ַ(0x00400 ~ 0x013FF), 4byte/sector
*                    dataΪ�������
*                    �������ز������          
************************************************************************************************
*/
INT8U EE_WordWrite(INT16U addr, INT16U data) 
{
    byte ret_value;

    byte Hi_add;
    word Lo_add;
        
    Hi_add = 0x00;
    Lo_add = addr & 0xffff;
       
    if((addr>=0x0400)&&(addr<=0x13ff))                     /* ����Ч��ַ��Χ�� */
    {
      ret_value = EE_Command(3, PROGRAM_D_FLASH, Hi_add, Lo_add, data,0,0,0);    /* ����ָ�� */
    }
    return ret_value;
}
/*
************************************************************************************************
*  Function:         EE_WordRead()
*  Description:      EEPROM���ֶ�ȡ����
*  Calls:            ��
*  Input:            INT16U addr
*  Output:           ��
*  Return:           INT16U
*  Others:           �������:addrΪ��ȡ����ʼ��ַ
*                    ����������ʼ��ַ��Ӧ������������          
************************************************************************************************
*/
INT16U EE_WordRead(INT16U addr) 
{
    return *((INT16U *)addr);
}