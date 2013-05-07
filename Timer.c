#include "define.h"

#ifndef __TIMERCFILE__
#define __TIMERCFILE__

#ifdef _MODBUSMASTER
#include "modmst.h"
#endif

#ifdef _MODBUSSLAVE
#include "modslv.h"
#endif

#ifdef _COM0
#include "CommPort0.h"
#endif

#ifdef _COM1
#include "CommPort1.h"
#endif

#ifdef _HSC
#include "HSC.h"
#endif

#ifdef _ADC
#include "adc.h"
#endif

#ifdef _KEY
#include "key.h"
#endif

#ifdef _BEEP
#include "beep.h"
#endif

#ifdef _MENU
#include "menu3.h"
#endif


#ifndef PCA_TIMER
void Init_Timer(){
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=0xDC;
	TL0=0x00;
	ET0=1;
	TR0=1;
}

void Timer() interrupt 1{	

	//WDT_CONTR = 0x3F;//看门狗复位

 	TH0=0xDC;//0xFC; 
	TL0=0x00;//0x6C; 

#ifdef _COM0
	COM0_Rcv(); //COM0接收结束
#endif

#ifdef _COM1
	COM1_Rcv(); //COM1接收结束
#endif

#ifdef _ADC
	adc();
#endif

#ifdef _KEY
	keydrv();
#endif

#ifdef _BEEP
	BeepDrv();
#endif

#ifdef _MENU
	MenuDrv();
#endif

}
#endif



//使用PCA为定时器
#ifdef PCA_TIMER
/*定义10mS PCA定时器常量*/
#define MS10L=0x00;
#define MS10H=0x24;

void Init_Timer(){//定义PCA为定时器及PWM使用！采用PCA Module 0为定时器,PCA为PWM.

	CMOD=0x80;//0x82; //以Fso/2作为时钟源
	CCON=0x00;
	CL=0x00;
	CH=0x00;
	CCAP0L=0x00;            //定时器方式工作
	CCAP0H=0x6C;
	CCAPM0=0x49;

	/***********************如果有NE555在板子上************************
	CCAP3L=0x70;            //PWM方式工作0x80=50% FREQ=FSO/2/256=21.6KHz
	CCAP3H=0x70;
	CCAPM3=0x42;			//PCA3工作于PWM方式的设定
	P2M0=0;					//P2.4强推挽输出
	P2M1=0x10;
	B20=0;					//初始化为低电平
	*******************************************************************/

	//ELVD=1;  	           //允许PCA及LVD中断
	CR=1;	                //PCA开始工作
}


void PCA_Timer(void) interrupt 7{
	//static int HT=0;
	//int tmpi;

	if (CCF0){								//如果是模块0产生的中断
		CCF0=0;								//软件清除中断标志

		//WDT_CONTR = 0x3F;					//看门狗复位

		CCAP0L=CCAP0L+MS10L;
		CCAP0H=CCAP0H+MS10H;
		
#ifdef COM0
		COM0_Rcv(); //COM0接收结束
#endif

#ifdef COM1
		COM1_Rcv(); //COM1接收结束
#endif

#ifdef HSC
		Hsc();		//模拟量采集
#endif

#ifdef KEY
		keydrv();
#endif
		BeepDrv();

	}
}
#endif

#endif //__TIMERCFILE__
