#include "define.h"

#ifndef __COM0CFILE__
#define __COM0CFILE__

xdata char COM0_rsBuf[COM0_RSBUF_LEN];	//定义COM0接收缓冲区
xdata int COM0_RcvCount = 0;				//定义COM0接收到的字符数
xdata int COM0_SndCount = 0;				//定义COM0要发送的字符数
xdata int COM0_SndPosi = 0;				//定义COM0要发送的当前字符索引
bit COM0_RcvCompleted = 0;			//定义COM0接收已完成标志
unsigned char COM0_CET=0;			//定义计数器

//data char COM0_PARITY=0;
#ifdef _MENU
#define COM0_PARITY SetValue.OddEven
#define ModSlv_SA SetValue.ModbusSA
#else
#define COM0_PARITY 1
#define ModSlv_SA 02
#endif

#ifdef _MODBUSSLAVE_COM0
#include "modslv.h"
#endif

#ifdef _BEEP
#include "beep.h"
#endif

#ifdef _MENU
#include "menu3.h"
#endif

#define SYSCLK 11059200
#define BAUDBASE SYSCLK / 384

code unsigned char BAUD[]= {-(BAUDBASE/1200),-(BAUDBASE/2400),-(BAUDBASE/4800),-(BAUDBASE/9600)};

void Init_COM0()
{
	COM0_RcvCount = 0;

	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD&=0x0F;
	TMOD|=0x20;
	
	//PCON |= 0x80;
	TH1=BAUD[SetValue.BaudRate]; //波特率9600 11.0592M 0xFD
	TL1=BAUD[SetValue.BaudRate]; //18.432M 0xFB

	SM0 = COM0_PARITY;
	SM1=1;

	SM2=0;
	REN=1;
	ES=1;
	TR1=1;
}

void Reinit_COM0()
{
	//EA = 0;

	TH1=BAUD[SetValue.BaudRate % 4]; //波特率9600 11.0592M 0xFD
	TL1=BAUD[SetValue.BaudRate % 4]; //18.432M 0xFB
	SM0 = COM0_PARITY;

	//EA = 1;
}

void COM0_Start_Sent()
{
	COM0_RcvCount = 0;
	ES=1;
	if (COM0_SndCount>0)
	{
		if (COM0_PARITY==1)
		{		//奇
			ACC = COM0_rsBuf[0];
			TB8=~P;
			SBUF=ACC;
		}
		else if (COM0_PARITY==2)
		{	//偶
			ACC = COM0_rsBuf[0];
			TB8=P;
			SBUF=ACC;
		}
		else
		{
			SBUF=COM0_rsBuf[0];
		}
		COM0_SndPosi=1;
	}
}


void COM0_Rcv()
{
	if (
#ifdef _MODBUSMASTER_COM0	
		(COM0_RcvCount==0 && COM0_CET>=COM0_TIMEOUT) || 
#endif		
		(COM0_RcvCount>0 && COM0_RcvCount<COM0_RSBUF_LEN && COM0_CET>=COM0_RCVTIMEOUT))
	{
		COM0_RcvCompleted = 1;
		//P2=COM0_RcvCount;
		COM0_CET = 0;
		//P0=COM0_CET;
		//REN = 0;	//接收完成，则禁止接收中断
	}
	else if (COM0_RcvCount>=COM0_RSBUF_LEN)
	{
		COM0_RcvCount=0;
		COM0_CET=0;
	}
	else
	{
		COM0_CET++;
	}
}

void Serial0() interrupt 4
{	//串行通讯中断 
	xdata unsigned char ch=0;

	COM0_CET=0;

	if (TI)
	{		//发送中断 
		TI=0;

		if(COM0_SndPosi<COM0_SndCount)
		{	//发送字符 
			ch = COM0_rsBuf[COM0_SndPosi++];
			
			if (COM0_PARITY==1)
			{		//奇
				ACC = ch;
				TB8=~P;
				SBUF=ch;
			}
			else if (COM0_PARITY==2)
			{	//偶
				ACC = ch;
				TB8=P;
				SBUF=ch;
			}
			else 
			{
				SBUF=ch;
			}
		}
		else
		{		//发送完毕 
			COM0_SndCount=0;
			REN=1;	//允许接收中断
		}
	}

	if (RI)
	{		//接收中断 
		RI=0;
		ch=SBUF;
		//Beep(1);
		if (COM0_RcvCount>0 && COM0_RcvCount<COM0_RSBUF_LEN)
		{	//接收剩余字节 
			COM0_rsBuf[COM0_RcvCount++]=ch;
		}
		else
		{
			if ((ch==
#ifdef _MODBUSMASTER_COM0
				COM0_rsBuf[0]
#else				
				ModSlv_SA
#endif				
				)||(ch==90))
			{ 	//判断接收到地址!
				COM0_rsBuf[0]=ch;
				COM0_RcvCount=1;
			}
			else
			{
				COM0_RcvCount=0;
			}
		}
	}
}

#endif	//__COM0CFILE__
