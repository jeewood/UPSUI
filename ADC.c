
#include "reg52.h"
#include "ModSlv.h"
#include "ModMst.h"
#include<math.h>

#include <intrins.H>

unsigned char ADC_Even = 0, AT = 0;
unsigned long tmpl = 0;

sbit LADC = P2^7;


void Init_ADC(unsigned char Chs) // 0000 0011 (0，1两个通道)
{
	P1M0 = 0;

#ifdef PWM1
	P1M0 |= 0x10;
#endif

#ifdef PWM0
	P1M0 |= 0x8;
#endif

	P1M1 = Chs;
	P1ASF = Chs;

	ADC_CONTR = 0xE0;
	_nop_();
}

unsigned char ChannelIdx = 0;
unsigned char SCnt = 0;

void Channel_Select(){

	SCnt = (ABRANCH_NUM>40)?42:ABRANCH_NUM+2;

	if (ChannelIdx>SCnt) ChannelIdx = 0;
	#ifdef TEST
		P2 |= 0x70;
		return ;
	#endif
	switch(ChannelIdx){

	//case 0 :	//选择参考电压
	//case 1 :	//选择正对地母线电压
	//case 2 :	//选择负对地母线电压

	case 3 : P2=0xF0;break;	//1111 0000 第1路传感器
	case 4 : P2=0xF4;break; //第2路传感器
	case 5 : P2=0xF2;break;
	case 6 : P2=0xF6;break;
	case 7 : P2=0xF1;break;
	case 8 : P2=0xF5;break;
	case 9 : P2=0xF3;break;
	case 10: P2=0xF7;break;

	case 11: P2=0xE8;break;	//1110 1000 第9路传感器
	case 12: P2=0xEC;break;
	case 13: P2=0xEA;break;
	case 14: P2=0xEE;break;
	case 15: P2=0xE9;break;
	case 16: P2=0xED;break;
	case 17: P2=0xEB;break;
	case 18: P2=0xEF;break;

	case 19: P2=0xD8;break;	//1101 1000 第17路传感器
	case 20: P2=0xDC;break;
	case 21: P2=0xDA;break;
	case 22: P2=0xDE;break;
	case 23: P2=0xD9;break;
	case 24: P2=0xDD;break;
	case 25: P2=0xDB;break;
	case 26: P2=0xDF;break;
	
	case 27: P2=0xB8;break;	//1011 1000	第25路传感器
	case 28: P2=0xBC;break;
	case 29: P2=0xBA;break;
	case 30: P2=0xBE;break;
	case 31: P2=0xB9;break;
	case 32: P2=0xBD;break;
	case 33: P2=0xBB;break;
	case 34: P2=0xBF;break;

	case 35: P2=0x78;break;	//0111 1000 第33路传感器
	case 36: P2=0x7C;break;
	case 37: P2=0x7A;break;
	case 38: P2=0x7E;break;
	case 39: P2=0x79;break;
	case 40: P2=0x7D;break;
	case 41: P2=0x7B;break;
	case 42: P2=0x7F;break; //第40路传感器
	}
}


unsigned char Get_Channel(){
	if (P2==0xFF){
		return 1;
	}else return 0;
}

void Channel_Close(){
	P2 = 0xFF;
}

int Volts = 0;
int Min = 0;
int Max = 0;
int Tmpix = 750;

void adc()
{
	unsigned long ADC_Result = 0;
	static unsigned char Times = 0;

	int tmpi = 0;

	if (ADC_Even==1)//关通道
	{
		if (!InTest) 
			Channel_Close();
		ADC_Even = 2;
		AT = 0;
	}
	else if (ADC_Even==2) //切换通道
	{
		if (AT>2)//2
		{
			if (InTest || Get_Channel()){
				Channel_Select();
				ADC_CONTR = 0xE0;
				switch(ChannelIdx)
				{
					case 0: ADC_CONTR |= 0; break;	//Vref	0
					case 1: ADC_CONTR |= 1; break;	//Vx+	1
					default:ADC_CONTR |= 5; break;	//Vst	5
				}
				_nop_();
				ADC_Even = 3;
			}else ADC_Even = 1;
			AT = 0;
		}
		else AT ++;
	}
	else if (ADC_Even==3)//延时30ms后开始采集
	{
		if (AT>1)//3
		{
			ADC_RES = 0;
			ADC_RESL = 0;
		
			ADC_CONTR |= 0x08;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			ADC_Even = 4;
			AT = 0;
		}else AT ++;
	}
	else if (ADC_Even==4)//等待电压稳定
	{
		if (AT>1)
		{
			ADC_Even=5;
			AT = 0;
		}else AT ++;
	}
	else if (ADC_Even==5)//等待采集完毕
	{
		if (ADC_CONTR & 0x10)
		{
			ADC_Result = ADC_RES;
			ADC_Result<<=2;
			ADC_Result |= ADC_RESL;

			ADC_CONTR &= 0xE7;
			_nop_();
			_nop_();
			_nop_();
			_nop_();

			if (Times==0)
			{
				Volts = (int)ADC_Result;
				Min = (int)ADC_Result;
				Max = (int)ADC_Result;
				Times++;
				ADC_CONTR |= 0x08;
				_nop_();
				_nop_();
				_nop_();
				_nop_();
			}
			else if(Times<12) //34
			{
				if ((int)ADC_Result>Max) Max = (int)ADC_Result;
				if ((int)ADC_Result<Min) Min = (int)ADC_Result;
				Volts += (int)ADC_Result;
				Times++;
				ADC_CONTR |= 0x08;
				_nop_();
				_nop_();
				_nop_();
				_nop_();
			}
			else	//计算值
			{
				Volts -= Max+Min;	//去掉最大值和最小值
							
				switch(ChannelIdx)	
				{
					case 0:			//正对地电压
						Value[ChannelIdx]= (int)(float)Volts*0.6;
						break;
					case 1:			//负对地电压
						Value[ChannelIdx]= (int)(float)Volts*0.6;

						if (Value[0]-Value[1]>100)	//母线绝缘电阻计算（正对地电阻）
						{
							tmpl = Value[1];
							tmpl *=470;
							tmpl /= Value[0]-Value[1];
							Value[2] = -tmpl;
						}
						else if (Value[1]-Value[0]>100) //母线绝缘电阻计算（正对地电阻）
						{
							tmpl = Value[0];
							tmpl *=470;
							tmpl /= Value[1]-Value[0];
							Value[2] = tmpl;
						}
						else
							Value[2] = 9999;

						ChannelIdx++;

						break;
					default:		//支路阻值计算
					{
						Volts -= 5120; //换成正负值

						Value[ChannelIdx] = 9999;

						if (Value[0] + Value[1] < 1650)
						{
							Tmpix = 375;
						}
						else
						{
							Tmpix = 750;
						}

						if (Value[2] != 9999 || (Volts<=4120 && Volts>=Tmpix) || (Volts>=-4120 && Volts<=-Tmpix))	//母线电阻异常或支路电流异常！
						{
							if (Volts>Tmpix/2)
							{
								Value[ChannelIdx] = floor(((float)Value[0]/((float)Volts/500))+0.5);
							}
							else if (Volts < -Tmpix/2)
							{
								Value[ChannelIdx] = floor((float)Value[1]/((float)Volts/500)+0.5);
							}
						}

						break;
					}
				}
				
				if (!InTest) ChannelIdx++; else ChannelIdx = InTest;
				ADC_Even = 1;
				Times = 0;
			}
		}
	}
	else ADC_Even = 1;
	//P0 = Times;
}

			
		
