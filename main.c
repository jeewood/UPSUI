#include "define.h"

#ifdef _COM0
#include "CommPort0.h"
//#include "CommPort0.c"
#endif

#ifdef _COM1
#include "CommPort1.h"
//#include "CommPort1.c"
#endif

#ifdef _MODBUSMASTER
#include "modmst.h"
//#include "modmst.c"
#endif

#ifdef _MODBUSSLAVE
#include "modslv.h"
//#include "modslv.c"
#endif

#ifdef _HSC
#include "HSC.h"
#include "HSC.c"
#endif

#ifdef _ADC
#include "ADC.h"
#include "ADC.c"
#endif

#ifdef _PWM1
#include "PWM1.h"
//#include "PWM1.c"
#endif

#ifdef _TIMER
#include "Timer.h"
//#include "Timer.c"
#endif

#ifdef _FLASH
#include "Flash.h"
//#include "Flash.c"
#endif

#ifdef _MENU
#include "menu3.h"
//#include "menu3.c"
#endif

#ifdef _BEEP
#include "beep.h"
//#include "beep.c"
#endif

#ifdef _KEY
#include "key.h"
//#include "key.c"
#endif

typedef union
{
	int W;
	unsigned char B[2];
	struct
	{
		unsigned char H, L;
	}BX;
}XByte;

xdata unsigned char Light = 13;
xdata unsigned char bLight = 13;
xdata unsigned char Key = 0;
bit isCom0Setted = 0;

void main()
{
	//EA=0;
#ifdef _TIMER
	//初始化通用定时器
	Init_Timer();
#endif

	//如定义COM1， 则初始化COM1
#ifdef _COM0
	Init_COM0();
#endif

	//如定义COM1， 则初始化COM1
#ifdef _COM1
	Init_COM1();
#endif

#ifdef _HSC
	Init_T0_Cnt();
#endif

#ifdef _ADC
	Init_ADC(0xE3);
#endif

#ifdef _KEY
	InitKeyDrv();
#endif

#ifdef _BEEP
	InitBeep();
#endif

#ifdef _PWM1
	PWM1_Out();
#endif

	EA=1;	//开全局中断

#ifdef _MENU	
	InitMenu();
#endif
	//Indicator(Light);
   	//LightOn(127);
#ifdef _WDT
	WDT_CONTR = 0x3E;
#endif
	ReadSetting();

	while(1)
	{

	//如定义COM0， 则初始化COM0

#ifdef _KEY
	Key = GetKey();
	
	if (Light == bLight)
	{
		if (Key==K_AT)
		{
			if (Light==7)
			{
				Light=13;
			}
			else if (Light==13)
			{
				Light = 12;
			}
			else
			{
				Light = 7;
			}
		}
		if (Key==K_OFF)
		{
			if (!Light) 
			{
				Light = 1;				
			}
			else
			{
				Light = 0;
			}
		}
	}
	else if (!Key)
	{
		bLight=Light;
	}
	
	if (Key)
	{
		Beep(1);
	}
	else if (Light==12)
	{
		Sound(100);
	}
	else
	{
		Sound(0);
		//Beep(0);
	}
	Indicator(Light);
	
#ifdef _MENU	
	Menu(Key);		
#endif

#endif	//_KEY defined

#ifdef _MODBUSMASTER

	if (ModMst_Idle && modscan == 0)
	{
		ModMst(2,3,0,12,
#ifdef _MENU
		(unsigned char*)&IValue
#else
		0
#endif
		);
	}
	else
	{
		ModMst_Rcv();
	}

	if (SettingChanged)
	{
		Reinit_COM0();
		SettingChanged = 0;
		LightOn(127);
	}

#endif	

#ifdef _MODBUSSLAVE
	ModSlv();
#endif	

#ifdef _WDT
	WDT_CONTR = 0x3E;
#endif

	}
}

