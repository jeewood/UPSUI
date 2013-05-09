#if !defined(__MODMST__)
#define __MODMST__

#include "define.h"

#ifdef _MODBUSMASTER_COM0
#include "CommPort0.h"
#else
#include "CommPort1.h"
#endif

#ifdef _MODBUSSLAVE //将Value[]包含进来
#include "modslv.h"
#endif

#include "crc16.h"
#include "lcd_dis.h"

typedef union
{
	int W;
	unsigned char B[2];
	struct
	{
		unsigned char H, L;
	}BX;
}WByte;

bit ModMst_Idle = 1;
char modscan = -3;

xdata char ModMst_ReadBit_Mask = 0;
xdata char * ModMst_RTN=0;

xdata WByte Tmpi;

xdata unsigned char ModMst_StartAddr = 32;
xdata unsigned char ModMst_MaxAddr = 15;

void ModMst_Rcv();

#ifdef _MODBUSMASTER_COM0
bit ModMst(char SA, char FN, unsigned int DA, unsigned int DL, unsigned char * DataOrRTN)
{
	if (!ModMst_Idle) return 0;
	data char i=0;

	COM0_SndCount = 0;
	ModMst_Idle = 0;
	COM0_rsBuf[0]=SA;
	COM0_rsBuf[1]=FN;
	Tmpi.W = DA;
	COM0_rsBuf[2]=Tmpi.BX.H;
	COM0_rsBuf[3]=Tmpi.BX.L;
	if (FN==1 || FN==2){
		Tmpi.W = (DL+7)/8;
		COM0_rsBuf[4]=Tmpi.BX.H;
		COM0_rsBuf[5]=Tmpi.BX.L;
		ModMst_ReadBit_Mask = DL % 8;	//用于接收到的数据，取有效的位！
	}else{
		COM0_rsBuf[4]=Tmpi.BX.H;
		COM0_rsBuf[5]=Tmpi.BX.L;
	}
	
	if (FN>=1 && FN<=4){
		Tmpi.W = crc16(COM0_rsBuf,6);
		COM0_rsBuf[6] = Tmpi.BX.L;
		COM0_rsBuf[7] = Tmpi.BX.H;
		
		ModMst_RTN->W = DataOrRTN;	//用于存放返回数据的指针
		
		COM0_SndCount = 8;
	}else if (FN==15 || FN ==16){
		COM0_rsBuf[6] = (FN==15)?(DL+7)/8 : (DL * 2) & 0xFF;
		for (i = 0;i<COM0_rsBuf[6];i++){
			COM0_rsBuf[7+i] = *(DataOrRTN+i);
		}
		Tmpi.W = crc16(COM0_rsBuf,7+i);
		COM0_rsBuf[7+(i++)]=Tmpi.BX.L;
		COM0_rsBuf[7+(i++)]=Tmpi.BX.H;
		COM0_SndCount = 7+i;
	}
	
	if (COM0_SndCount>0){
		COM0_Start_Sent();
	}
	return 1;
}

void ModMst_Rcv(){
	xdata int r = 9999;
	xdata bit f = 0;
	if (COM0_RcvCompleted){
		if (!COM0_RcvCount)
		{
			*ModMst_RTN = 9998;
		}
		else if (crc16(COM0_rsBuf,COM0_RcvCount)==0)
		{
			Tmpi.BX.H = COM0_rsBuf[3];
			Tmpi.BX.L =	COM0_rsBuf[4];
			if (Tmpi.BX.H & 0x80)
			{
				f = 1;
				Tmpi.BX.H &= ~0x80;
			}

			Tmpi.W /= 10;

			if (Tmpi.W>50)
			{
				if (f)
				{
					r = (int)(float)Value[1]*100.0 / -Tmpi.W;
					if (r<-1000) r = 9999;
				}
				else
				{
					r = (int)(float)Value[0]*100.0 / Tmpi.W;
					if (r>1000) r = 9999;
				}
			}
			*ModMst_RTN = r;
		}

		COM0_RcvCount = 0;
		COM0_RcvCompleted = 0;

		ModMst_Idle = 1;
	}	
}

#else

bit ModMst(char SA, char FN, unsigned int DA, unsigned int DL, unsigned char * DataOrRTN)
{
	xdata char i=0;
	if(!ModMst_Idle) return 0;
	//MM_InWaiting = 1;
	ModMst_Idle = 0;
	COM1_SndCount = 0;
	COM1_RcvCount = 0;

	ModMst_RTN = 0;

	COM1_rsBuf[0]=SA;
	COM1_rsBuf[1]=FN;
	Tmpi.W = DA;
	COM1_rsBuf[2]=Tmpi.BX.H;
	COM1_rsBuf[3]=Tmpi.BX.L;
	if (FN==1 || FN==2)
	{
		Tmpi.W = (DL+7)/8;
		COM1_rsBuf[4]=Tmpi.BX.H;
		COM1_rsBuf[5]=Tmpi.BX.L;
		ModMst_ReadBit_Mask = DL % 8;	//用于接收到的数据，取有效的位！
	}
	else
	{
		Tmpi.W = DL;
		COM1_rsBuf[4]=Tmpi.BX.H;
		COM1_rsBuf[5]=Tmpi.BX.L;
	}

	if (FN>=1 && FN<=6)
	{
		Tmpi.W = crc16(COM1_rsBuf,6);
		COM1_rsBuf[6] = Tmpi.BX.L;
		COM1_rsBuf[7] = Tmpi.BX.H;
		
		ModMst_RTN = DataOrRTN;	//用于存放返回数据的指针
		
		COM1_SndCount = 8;
	}
	else if (FN==15 || FN ==16)
	{
		COM1_rsBuf[6] = (FN==15)?(DL+7)/8 : (DL * 2) & 0xFF;
		for (i = 0;i<COM1_rsBuf[6];i++)
		{
			COM1_rsBuf[7+i] = *(DataOrRTN+i);
		}
		Tmpi.W = crc16(COM1_rsBuf,7+COM1_rsBuf[6]);
		COM1_rsBuf[7+COM1_rsBuf[6]]=Tmpi.BX.L;//CRC_CODE & 0xFF;
		COM1_rsBuf[8+COM1_rsBuf[6]]=Tmpi.BX.H;//(CRC_CODE>>8) & 0xFF;
		COM1_SndCount = COM1_rsBuf[6]+9;
	}
	
	if (COM1_SndCount>0)
	{
		COM1_Start_Sent();
	}
	return 1;
}

void ModMst_Rcv(){
	//本函数中插入电阻值计算部分电流值>0使用Value[1]/电流值，否则使用Value[2]/电流值。
	if (COM1_RcvCompleted)
	{
		xdata char i = 0;
		//ShowChar(0,0,COM1_RcvCount,1);
		if (COM1_RcvCount==0)
		{
			*ModMst_RTN = 0xFF;
		}
		else if (crc16(COM1_rsBuf,COM1_RcvCount)==0 && COM1_rsBuf[1]>=1 && COM1_rsBuf[1]<=4)
		{
			for(i=0;i<COM1_rsBuf[2];i++)
			{
				*(ModMst_RTN+i) = COM1_rsBuf[i+3];
			}
		}

		COM1_RcvCount = 0;
		COM1_RcvCompleted = 0;

		ModMst_Idle = 1;
	}	
}
#endif

#endif
