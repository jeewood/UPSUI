#ifndef	__LCD_DIS_h__
#define	__LCD_DIS_h__
//	write your header here
extern void TimeDelay(unsigned int Timers);
extern void LCD_Init(void);
extern void SPI_SSSet(unsigned char Status);
extern void SPI_Send(unsigned char Data);

extern void FontSet(unsigned char Font_NUM,unsigned char Color);
extern void FontSet_cn(unsigned char Font_NUM,unsigned char Color);
extern void PutChar(unsigned char x,unsigned char y,unsigned char a);
extern void PutString(unsigned char x,unsigned char y,unsigned char *p);
extern void PutChar_cn(unsigned char x,unsigned char y,unsigned char * GB);
extern void PutString_cn(unsigned char x,unsigned char y,unsigned char *p);
extern void SetPaintMode(unsigned char Mode,unsigned char Color);
extern void PutPixel(unsigned char x,unsigned char y);
extern void Line(unsigned char s_x,unsigned char  s_y,unsigned char  e_x,unsigned char  e_y);
extern void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode);
extern void Rectangle(unsigned char left, unsigned char top, unsigned char right,
				 unsigned char bottom, unsigned char mode);
extern void ClrScreen(void);
extern void PutBitmap(unsigned char x,unsigned char y,unsigned char width,unsigned char high,unsigned char *p);
extern void FontMode(unsigned char Cover,unsigned char Color);
extern void ShowChar(unsigned char x,unsigned char y,unsigned char a,unsigned char type) ;
extern void ShowShort(unsigned char x,unsigned char y,unsigned short a,unsigned char type) ;
extern void SetBackLight(unsigned char Deg);
//================jwz added================
extern unsigned char cStrLen(char * str);
extern unsigned char StrLen(char * str);
extern unsigned char cPrint12(unsigned char cfs,unsigned char efs,char Line,char Column,char* str,unsigned char i);
extern unsigned char PrintNum(unsigned char cfs,unsigned char efs,char Line,char Column,unsigned short num,unsigned char i);
extern void Print6(unsigned char fs,char Line,char Column, char * str,unsigned char i);
extern void CPrint(unsigned char cfs, unsigned char efs, char y, char x, char* str,unsigned char inv);
extern unsigned char cStrLen(char * str);
extern unsigned char StrLen(char * str);
extern unsigned char Print(char x,char y,char* str,char i);
#endif	//__LCD_DIS_h__
