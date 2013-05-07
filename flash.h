#ifndef __FLASHHFILE__
#define __FLASHHFILE__

extern unsigned char Read_Byte(unsigned int addr);//addr从0开始
extern void Erase_Section(unsigned int addr);
extern void Write_Byte(unsigned int addr,unsigned char ch);//addr从0开始
extern EEROM_BASE_ADDR;

#endif //__FLASHHFILE__
