/**********************************************************************************************
*              Shanghai H&D EV Battery Co.,Ltd.
*             No.805 Chungui Road, Huangdu Town,
*             Jiading District, Shanghai 201804
*                         P.R. China
*
*             (c) Copyright 2011, H&D, ShangHai    
*                    All Rights Reserved
*
*File Name :     flash.h
*Programmer(s) : Zeng Qunxin
*Descriprion :
*History :
*    <version>             <desc>                <time>       <author>
* 1.   V1.0             Build this moudle          2011-12-14   Zeng Qunxin
**********************************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "TYPEDEFINE.h"
/***************************************Macro define******************************************/
/**** P-Flash and D-Flash Commands ****/
#define ERASE_VERIFY_ALL_BLOCKS  0x01 
/* Verify that all program and data Flash blocks are erased. */
/* CCOBIX end = 0 */
/* CCOB Params - NONE */
/* MGSTAT set if fault */

#define ERASE_VERIFY_BLOCK       0x02      
/* Verify that a Flash block is erased. */
/* CCOBIX end = 0 */
/* CCOB Params - gpage */
/* MGSTAT set if fault */

#define ERASE_ALL_BLOCKS         0x08 
/* Erase all program and data Flash blocks.
   An erase of all Flash blocks is only possible when the FPLDIS, FPHDIS, and FPOPEN
   bits in the FPROT register and the EPDIS and EPOPEN bits in the EPROM register are
   set prior to launching the command. */
/* CCOBIX end = 0 */
/* CCOB Params - NONE */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define UNSECURE_FLASH           0x0B 
/*Supports a method of releasing MCU security by erasing all program and data Flash
  blocks and verifying that all program and data Flash blocks are erased. */
/* CCOBIX end = 0 */
/* CCOB Params - NONE */
/* MGSTAT set if fault */

#define SET_USER_MARGIN_LEVEL    0x0D 
/*Specifies a user margin read level for all program Flash blocks. */
/* CCOBIX end = 1 */
/* CCOB Params - gpage, level setting (0-2) in CCOB[1] */
/* ACCERR set if invalid level */

#define SET_FIELD_MARGIN_LEVEL   0x0E 
/*Specifies a field margin read level for all program Flash blocks (special modes only). */
/* CCOBIX end = 1 */
/* CCOB Params - gpage, level setting (0-4) in CCOB[1] */
/* ACCERR set if invalid level */

/* **** P-Flash Only Commands ****/
#define ERASE_VERIFY_P_FLASH_SECTION 0x03  
/*Verify that a given number of words starting at the address provided are erased. */
/* CCOBIX end = 2 */
/* CCOB Params - global address, number of phrases in CCOB[2]*/
/* MGSTAT set if fault */

#define READ_ONCE	               0x04  
/* Read a phrase from a dedicated 64 word area in a hidden region of a programFlash block
   that was previously programmed using the Program Once command. */
/* CCOBIX end = 1 */
/* CCOB Params - read once index (0-3) in CCOB[1], phrase in CCOB [5:2] */
/* returns phrase in CCOB [4:1] */

#define PROGRAM_P_FLASH          0x06 
/* Program a phrase in a program Flash block and any previously loaded phrases for any
   other program Flash block (see Load Data Field command). */
/* CCOBIX end = 5 */
/* CCOB Params - global address, phrase in CCOB [5:2] */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define PROGRAM_ONCE             0x07 
/* Program a dedicated 64 word area in a hidden region of a program Flash block that is
   allowed to be programmed only once. */
/* CCOBIX end = 5 */
/* CCOB Params - read once index (0-3) in CCOB[1], phrase in CCOB [5:2] */
/* MGSTAT set if fault */

#define ERASE_FLASH_BLOCK      0x09 
/* Erase a program Flash block.
   An erase of the full program Flash block is only possible when FPLDIS, FPHDIS and
   FPOPEN bits in the FPROT register are set prior to launching the command. */
/* CCOBIX end = 1 */
/* CCOB Params - global address */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define ERASE_P_FLASH_SECTOR 0x0A 
/* Erase all bytes in a program Flash sector. */
/* CCOBIX end = 1 */
/* CCOB Params - global address */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define VERIFY_BACKDOOR_ACCESS_KEY 0x0C 
/*Supports a method of releasing MCU security by verifying a set of security keys. */
/* CCOBIX end = 4 */
/* CCOB Params - backdoor key in CCOB [1:4] */
/* ACCERR set if not verified */

/**** D-Flash Only Commands ****/
#define ERASE_D_FLASH_BLOCK      0x09 
/* Erase a program Flash block.
   An erase of the full program Flash block is only possible when DPOPEN bit in the DFPROT
     register is set prior to launching the command. */
/* CCOBIX end = 1 */
/* CCOB Params - global address */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define ERASE_VERIFY_D_FLASH_SECTION 0x10 
/* Verify that a given number of words starting at the address provided are erased. */
/* CCOBIX end = 2 */
/* CCOB Params - global address of first word, number of words to verify CCOB[2]*/
/* MGSTAT set if fault */

#define PROGRAM_D_FLASH         0x11 
/* Program up to four words in the data Flash block (see Load Data Field command). */
/* CCOBIX end = 2 */
/* CCOB Params - global address, up to 4 data words in CCOB [2:5] */
/* MGSTAT set if fault, EPVIOL / ACCERR set where appropriate */

#define ERASE_D_FLASH_SECTOR    0x12 
/* Erase all bytes in a data Flash sector. */
/* CCOBIX end = 2 */
/* CCOB Params - global address */
/* MGSTAT set if fault, EPVIOL  set where appropriate */
/*********************************Function declaration****************************************/
INT8U EE_Command(INT8U params, INT8U command, INT8U ccob0, INT16U ccob1, INT16U ccob2, INT16U ccob3, INT16U ccob4, INT16U ccob5);
INT8U EE_DwordErase(INT16U addr); 
INT8U EE_WordWrite(INT16U addr, INT16U data); 
INT16U EE_WordRead(INT16U addr);          
 
#endif