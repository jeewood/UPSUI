#include "define.h"

#ifndef __COM1CFILE__
#define __COM1CFILE__

#ifdef _COM1
xdata char COM1_rsBuf[COM1_RSBUF_LEN];	//定义COM0接收缓冲区
//xdata char COM1_sBuf[COM1_RSBUF_LEN];

xdata int COM1_RcvCount = 0;				//定义COM0接收到的字符数
xdata int COM1_SndCount = 0;				//定义COM0要发送的字符数
xdata int COM1_SndPosi = 0;				//定义COM0要发送的当前字符索引
bit COM1_RcvCompleted = 0;			//定义COM0接收已完成标志
bit COM1_SndCompleted = 0;			//定义COM0发送已完成标志
xdata unsigned char COM1_CET=0;			//定义计数器

xdata char COM1_PARITY=1;

#ifdef _MODBUSSLAVE_COM1
#include "modslv.h"
#endif

#include "CommPort1.h"

void Init_COM1()
{

	COM1_RcvCount = 0;	
	BRT =0xFD;	//Fosc /12 /(256-BRT) BRTx12 = 0; = 11059200 /12 /(256-X) = 9600*32 :X = 253 = 0xFD

	if (COM1_PARITY) //10位长度
	{
		S2CON = 0xC0;
	}
	else
	{
		S2CON = 0x40; 
	}
					  //1为9位  可变BR 
					  //S2SM0  S2SM1  S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI
					  //1        1     0       0      0      0      0     0
	AUXR = 0x10; //T0x12, T1x12, UART_M0x6, BRTR, S2SMOD, BRTx12, EXTRAM, S1BRS

	IE2 |= 0x01; //Enable ES2=1;

}

void COM1_Start_Sent()
{
	IE2 |= 0x01; //Enable ES2=1;
	COM1_RcvCount=0;
	if (COM1_SndCount)
	{
		if (COM1_PARITY==1)
		{		//奇
			ACC=COM1_rsBuf[0];
			if (P)
			{
				S2CON |=8;
				S2BUF=COM1_rsBuf[0];
			}
			else
			{
				S2CON &=0xF7;
				S2BUF=COM1_rsBuf[0];
			}
		}
		else if (COM1_PARITY==2)
		{	//偶
			ACC=COM1_rsBuf[0];
			if (P)
			{
				S2CON &=0xF7;
				S2BUF=COM1_rsBuf[0];
			}
			else
			{
				S2CON |=8;
				S2BUF=COM1_rsBuf[0];
			}
		}
		else
		{ 
			S2BUF=COM1_rsBuf[0];
		}
		COM1_SndPosi=1;
		//S2CON &=0xEF; //S2REN=1; 禁止接收中断
	}
}

void COM1_Rcv()
{
	if (COM1_RcvCount==0 && COM1_CET>=COM1_TIMEOUT)
	{
		COM1_RcvCompleted = 1;
		COM1_CET = 0;
	}
	else if (COM1_RcvCount>0 && COM1_CET>=COM1_RCVTIMEOUT)
	{
		COM1_RcvCompleted = 1;
		COM1_CET = 0;
		//S2CON &=0xEF; //S2REN=1; 禁止接收中断
	}
	else
	{
		COM1_CET++;
	}
	//P0 = COM1_CET;
}

void Serial1() interrupt 8	//串行通讯中断
{ 
	data unsigned char ch=0;

	COM1_CET=0;
	
	if (S2CON & 0x02)	//TI=1
	{		//发送中断 
		S2CON &= 0xFD;	//TI = 0;
		if(COM1_SndPosi<COM1_SndCount)
		{	//发送字符
			
			ch=COM1_rsBuf[COM1_SndPosi++];
			if (COM1_PARITY==1)
			{		//奇
				ACC = ch;
				if (P)
				{
					S2CON |=8;
					S2BUF=ch;
				}
				else
				{
					S2CON &=0xF7;
					S2BUF=ch;
				}
			}
			else if (COM1_PARITY==2)
			{	//偶
				ACC = ch;
				if (P)
				{
					S2CON &=0xF7;
					S2BUF=ch;
				}
				else
				{
					S2CON |= 8;
					S2BUF=ch;
				}
			}
			else 
				S2BUF=ch;
			
		}
		else
		{		//发送完毕 
			COM1_SndCount=0;
			S2CON |=0x10; //S2REN=1; 允许接收中断
		}
	}
	else	//RI=1
	{		//接收中断 
		S2CON &= 0xFE;	//RI = 0;
		ch=S2BUF;		
		if (COM1_RcvCount>0 && COM1_RcvCount<COM1_RSBUF_LEN)//接收剩余字节 
		{
			COM1_rsBuf[COM1_RcvCount++]=ch;
		}
		else
		{
			if ((ch==COM1_rsBuf[0])) //判断接收到地址!
			{
				COM1_rsBuf[0] = ch;
				COM1_RcvCount=1;
			}
			else
			{
				COM1_RcvCount=0;
			}
		}
	}
}

#endif
#endif	//__COM0CFILE__