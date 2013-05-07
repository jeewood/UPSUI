#include "define.h"

#ifndef __PWM1CFILE__
#define __PWM1CFILE__

void PWM1_Out()
{
    CMOD    =   8;   	// CIDL - - - - CPS1 CPS0 ECF  Setup PCA Timer
                        // CPS2 CPS1 CPS0 = 5, Fosc/2 is PCA/PWM clock
                        // 11059200/2/256 = 6000
    CL      =   0x00;
    CH      =   0x00;
    CCAP1L  =   0x80;   //Set the initial value same as CCAP1H
    CCAP1H  =   0x80;   //50% Duty Cycle
    CCAPM1  =   0x42;   //0100,0010 Setup PCA module 1 in 8BIT PWM, P3.7
    CR      =   1;      //Æô¶¯ PCA/PWM ¶¨Ê±Æ÷
}

#endif //__PWM1CFILE__
