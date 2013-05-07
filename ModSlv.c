#if !defined(__MODSLV__)
#define __MODSLV__

#include "define.h"

#ifdef _COM0
#include "CommPort0.h"
#endif

#ifdef _COM1
#include "CommPort1.h"
#endif

#include "crc16.h"

#include "flash.h"
#include "adc.h"

#include "menu3.h"

//extern char Adjust;		//传感器校零标志变量

union
{
	int W;
	unsigned char B[2];
	struct
	{
		unsigned char H, L;
	}BX;
}Tmpx;

//int Value[291];
//Value[0]=正母线电压
//Value[1]=负母线电压
//Value[2]=正母线电阻
//Value[3]=负母线电阻
//Value[4]=支路1...

#ifdef _MODBUSSLAVE_COM0

#define FUNC3
#define FUNC6

void ModSlv(){
	//data unsigned int CRC_CODE=0;
	xdata unsigned char back_flash[20];
	xdata unsigned char i=0;
	xdata unsigned char DAH,DAL,DLH,DLL;
	
	if (COM0_RcvCompleted)
	{
		if (COM0_RcvCount>0 && crc16(COM0_rsBuf,COM0_RcvCount)==0)
		{
			DAH = COM0_rsBuf[2];
			DAL = COM0_rsBuf[3];
			DLH = COM0_rsBuf[4];
			DLL = COM0_rsBuf[5];

			switch(COM0_rsBuf[1])
			{
#ifdef FUNC1
			case 1:case 2:
				DL=(DL+7)/8;
				COM0_rsBuf[2] = DL & 0xFF;
				MoveData(&Value[DA],&COM0_rsBuf[3],DL);
				COM0_SndCount = DL + 3;

				Tmpx.W = crc16(COM0_rsBuf,COM0_SndCount);
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.L;//CRC_CODE & 0xFF;
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.H;//(CRC_CODE >> 8) & 0xFF;

				break;
#endif
#ifdef FUNC3
			case 3:case 4:
				COM0_rsBuf[2] = DLL*2;
				
				if (DAH==0x40 && DLL <=0x0A)
				{
					for (i=0;i<DLL*2;i++)
					{
						COM0_rsBuf[3+i]=Read_Byte(i+DAL*2);	//读取设定值
					}
				}
				else if (DAH==0 && DAL<=0xFF)
				{
					for (i=0;i<COM0_rsBuf[2];i++)
					{
						COM0_rsBuf[3+i] = *((char*)&IValue+i);
					}
				}
				else if (DAH==0x30 && DAL>=0x00 && DAL<= 0xFF && DLL==1)
				{
					COM0_rsBuf[2] = 2;
					//Tmpx.W = Value[InTest];
					COM0_rsBuf[3] = Tmpx.BX.H;
					COM0_rsBuf[4] = Tmpx.BX.L;
				}
				else break;
				
				COM0_SndCount = DLL*2 + 3;

				Tmpx.W = crc16(COM0_rsBuf,COM0_SndCount);
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.L;//CRC_CODE & 0xFF;
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.H;//(CRC_CODE >> 8) & 0xFF;
				
				break;
#endif
#ifdef FUNC5				
			case 5:
				COM0_SndCount = COM0_RcvCount;
				break;
#endif
#ifdef FUNC6
			case 6:
				if (DAH==0x40 && DAL<=0x0A)							//参数设定
				{
					for (i=0;i<20;i++)back_flash[i]=Read_Byte(i);	//备份设定值
					
					Erase_Section(0);								//删除扇区
					if (DAL==2)
					{
						if (DLH>40) DLH = 40;
						back_flash[4]=DLH;
						back_flash[5]=DLL;
					}
					else
					{
						back_flash[DAL*2]=DLH;
						back_flash[DAL*2+1]=DLL;
					}
				
					for (i=0;i<20;i++)Write_Byte(i,back_flash[i]);	//回写设定值
					
					COM0_SndCount = COM0_RcvCount;
				}
				else
				if (DAH == 0x50 && DAL == 0x00 && DLH == 0x00 && DLL == 0x00)	//通讯传感器校零
				{
					COM0_SndCount = COM0_RcvCount;
				}
					
				break;
#endif
#ifdef FUNC15				
			case 15:
				//ModSlv Func 15
				COM0_SndCount = 6;
				Tmpx.W = crc16(COM0_rsBuf,COM0_SndCount);
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.L;//CRC_CODE & 0xFF;
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.H;//(CRC_CODE >> 8) & 0xFF;
				break;
#endif
#ifdef FUNC16
			case 16:
				//ModSlv Func 16
				COM0_SndCount = 6;
				Tmpx.W = crc16(COM0_rsBuf,COM0_SndCount);
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.L;//CRC_CODE & 0xFF;
				COM0_rsBuf[COM0_SndCount++] = Tmpx.BX.H;//(CRC_CODE >> 8) & 0xFF;
				break;
#endif
			}
		}

		COM0_RcvCompleted = 0;
		COM0_RcvCount = 0;

		if (COM0_SndCount>0)
		{
			COM0_Start_Sent();
		}
	}
}

#else

void ModSlv(){
	if (COM1_RcvCompleted){
		switch(COM1_rsBuf[1]){
		case 1:case 2:
			//ModSlv Func 1,2
			break;
		case 3:case 4:
			//ModSlv Func 3,4
			break;
		case 5:
			//ModSlv Func 5
			break;
		case 6:
			//ModSlv Func 6
			break;
		case 15:
			//ModSlv Func 15
			break;
		case 16:
			//ModSlv Func 16
			break;
		}
	}
}

#endif
#endif