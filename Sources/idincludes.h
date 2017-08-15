#ifndef __IDINCLUDES_h
#define __IDINCLUDES_h

#include "TYPEDEFINE.h"


#define IDNumber  0//EE_Read(0x0708)


#define CCP_DTO_ID    idnum[IDNumber][1]  
#define CCP_CRO_ID    idnum[IDNumber][0]

      INT16U  idnum[25][2]={ 
               {0x450,0x10F},
               {0x451,0x11F},
               {0x452,0x12F}, 
               {0x453,0x13F}, 
               {0x454,0x14F}, 
               {0x455,0x15F},
               {0x456,0x16F}, 
               {0x457,0x17F}, 
               {0x458,0x18F}, 
               {0x459,0x19F},
               {0x45A,0x1AF}, 
               {0x45B,0x1BF}, 
              
               {0x461,0x21F},
               {0x462,0x22F}, 
               {0x463,0x23F}, 
               {0x464,0x24F},
               {0x465,0x25F},
               {0x466,0x26F}, 
               {0x467,0x27F}, 
               {0x468,0x28F},     
               {0x469,0x29F},
               {0x46A,0x2AF}, 
               {0x46B,0x2BF},
               
               {0x46C,0x2CF}, 
               {0x46D,0x2DF}

 }; 
 
#endif    