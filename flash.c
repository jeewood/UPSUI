#include "define.h"

#ifndef __FLASHCFILE__
#define __FLASHCFILE__
const IAP_READ  = 1;
const IAP_WRITE = 2;
const IAP_ERASE = 3;

const EEROM_BASE_ADDR=0;//x2000;

unsigned char Read_Byte(unsigned int addr){

	xdata unsigned char tmpb =0;
	addr += EEROM_BASE_ADDR;

	EA=0;
	IAP_CONTR=0x83; //5MHz(0x83) 10MHz(0x82) 20MHz(0x81) 40MHz(0x80)
	IAP_CMD  = IAP_READ;
	IAP_ADDRH=addr>>8;
	IAP_ADDRL=addr&0xFF;
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0xA5;
	_nop_();
	tmpb=IAP_DATA;

	IAP_CONTR = 0;
	IAP_CMD   = 0;
	IAP_TRIG  = 0;
	EA=1;

	return tmpb;
}

void Write_Byte(unsigned int addr,unsigned char ch){

	addr += EEROM_BASE_ADDR;
	
	EA=0;
	IAP_CONTR = 0x83;
	IAP_CMD   = IAP_WRITE;
	IAP_ADDRH = addr>>8;
	IAP_ADDRL =	addr&0xFF;
	IAP_DATA = ch;
	IAP_TRIG  = 0x5A;
	IAP_TRIG  = 0xA5;
	_nop_();

	IAP_CONTR = 0;
	IAP_CMD   = 0;
	IAP_TRIG  = 0;
	EA = 1;
}

void Erase_Section(unsigned int addr){

	addr += EEROM_BASE_ADDR;

	EA = 0;
	IAP_CONTR = 0x83;
	IAP_CMD   = IAP_ERASE;
	IAP_ADDRH = addr>>8;
	IAP_ADDRL = addr&0xFF;
	IAP_TRIG  = 0x5A;
	IAP_TRIG  = 0xA5;
	_nop_();

	IAP_CONTR = 0;
	IAP_CMD   = 0;
	IAP_TRIG  = 0;
}

#endif //__FLASHCFILE__
