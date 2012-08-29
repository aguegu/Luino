//================================================================================
//		LogicGreen Techonologes
//
// Description:
//	C header file for LGT8F0XA Series Microcontroller
// Version:
//	1.1
//=================================================================================
// Note:

FOR WINAVR
1. iolgt8f0xa.h是库头文件io.h和iomxx4的补充, 它定义了LGT8F0XA所特有的寄存器和中断向量.
2. 在工程中同时包含头文件io.h和iolgt8f0xa.h就可包所有LGT8F08XA的寄存器和中断向量定义,
   (iomxx4.h在io.h中根据MCU型号自动包含).

FOR ICCAVR
1. iolgt8f0xa.h是库头文件iom164v.h的补充, 它定义了LGT8F0XA所特有的寄存器和中断向量.
2. 在工程中同时包含头文件iom164v.h和iolgt8f0xa.h就可包所有LGT8F08XA的寄存器和中断向量定义.
