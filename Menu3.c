#include "define.h"
#ifndef __MENUCFILE__
#define __MENUCFILE__

#include "menu3.h"
#include "LCD_Dis.h"

#include "flash.h"

#ifdef _KEY
#include "key.h"
#endif

#include "modmst.h"

xdata CValue IValue={0};
xdata CValue Vadj={0};
xdata SValue SetValue = {0};

typedef struct _strArr
{
	char len;
	char**sa;
}strArr;

enum{
	CHAR,
	INTEGER,
	STRING,
	FUNCTION,
	CONST,
	MENU
};

#define TYPE(sid,id) (((unsigned char)sid<<5) | ((unsigned char)id &0x1F))

typedef struct _MenuItem
{
	char id;
	char *ItemStr;
	char ItemType; 	//高3位用于显示时ptr对应的类型
				  	//｛CHAR,INTEGER为数字,STRING为预定义的字符串｝
					//低5位当高3位为MENU时,低5位为菜单项数(最多31个)
	void *ptr;
	char mcnt;
}MenuItem;

typedef struct
{
	char inEdit;
	char id;
	char bid; //backup id

	MenuItem *mm;
	MenuItem *sm;
}MENUPARAM;

MENUPARAM mr={0};

MenuItem * pMainMenu = 0;

typedef void (*FUNC)();

#define CHAR(c)		(&c)
#define INT(i)		(&i)
#define STR(s)		(s)
#define FUNC(f)		(&f)

#define CCHAR(c)	(*(char*)(c))
#define CINT(i)		(*(int*)(i))
#define CSTR(s)		((char*)(s))
#define CFUNC(f)	((FUNC)(f))
#define CMENU(m) 	((MenuItem*)(m)) //(mr.ptr)+

xdata unsigned int LightCnt = 0;
bit isBackLightOn = 0;
bit SettingChanged = 1;

xdata unsigned int DspCnt = 0;
void MenuDrv(void)
{
	if (DspCnt>0)DspCnt++;
	if (LightCnt>0 && LightCnt != 255) LightCnt--;
}

void LightOn(unsigned char Deg)
{
	SetBackLight(Deg);
	isBackLightOn = 1;
	LightCnt = SetValue.BKLtime * 100;
}

void LightOff()	//关闭背光
{
	SetBackLight(0);
	isBackLightOn = 0;
}

code char *WorkMode[]={"在线式","离线式"};
code char *StartMode[]={"自动","手动"};

code char *BaudRate[]={"1200","2400","4800","9600"};
code char *DataBits[]={"8","7"};
code char *StopBits[]={"1","2"};
code char *OddEven[]={"无","奇","偶"};

code strArr SettingArray[]=
{
	{2,WorkMode},		//0
	{2,StartMode},		//1
	{4,BaudRate},	    //2
	{2,DataBits},		//3
	{2,StopBits},		//4
	{3,OddEven}			//5
};

code char* FaultInfo[]=
{
	{"交流失电"},
	{"旁路失电"},
	{"电池过压"},
	{"电池欠压"},
	{"旁路失电"},
	{"旁路失电"}
};

#define SSCNT 3
code MenuItem SysSetting[]=
{
	{-1,"系统设置",MENU,&pMainMenu,SSCNT},
	{0," 工作方式:",CHAR,CHAR(SetValue.wMode),STRING},
	{1," 启动模式:",CHAR,CHAR(SetValue.sMode),STRING},
	{-1," 背光时间:",CHAR,CHAR(SetValue.BKLtime),CHAR}
};

#define CSCNT 5
code MenuItem ComSetting[]=
{
	{-1,"通迅设置",MENU,&pMainMenu,CSCNT},
	{-1," 站  址:",CHAR,CHAR(SetValue.ModbusSA),CHAR},
	{2," 波特率:",CHAR,CHAR(SetValue.BaudRate),STRING},
	{3," 数据位:",CHAR,CHAR(SetValue.DataBits),STRING},
	{4," 停止位:",CHAR,CHAR(SetValue.StopBits),STRING},
	{5," 校验位:",CHAR,CHAR(SetValue.OddEven),STRING}
};

#define IMCNT 11
code MenuItem Info[]=
{
	{-1,"系统信息",MENU,&pMainMenu,IMCNT},
	{-1," 输入电压:",INTEGER,INT(IValue.ACInV),CONST},
	{-1," 输入电流:",INTEGER,INT(IValue.ACInI),CONST},
	{-1," 输入频率:",INTEGER,INT(IValue.ACInF),CONST},
	{-1," 输出电压:",INTEGER,INT(IValue.InvV),CONST},
	{-1," 负载电流:",INTEGER,INT(IValue.LoadI),CONST},
	{-1," 输出频率:",INTEGER,INT(IValue.InvF),CONST},
	{-1," 输出功率:",INTEGER,INT(IValue.Power),CONST},
	{-1," 功率因素:",INTEGER,INT(IValue.Factor),CONST},
	{-1," 电池电压:",INTEGER,INT(IValue.BatV),CONST},
	{-1," 充电电流:",INTEGER,INT(IValue.BatI),CONST},
	{-1," 充电状态:",INTEGER,INT(IValue.ChargeStatus),CONST}
};

code MenuItem InfoAdj[]=
{
	{-1,"参数校正",MENU,&pMainMenu,IMCNT},
	{-1," 输入电压:",INTEGER,INT(Vadj.ACInV),INTEGER},
	{-1," 输入电流:",INTEGER,INT(Vadj.ACInI),INTEGER},
	{-1," 输入频率:",INTEGER,INT(Vadj.ACInF),INTEGER},
	{-1," 输出电压:",INTEGER,INT(Vadj.InvV),INTEGER},
	{-1," 负载电流:",INTEGER,INT(Vadj.LoadI),INTEGER},
	{-1," 输出频率:",INTEGER,INT(Vadj.InvF),INTEGER},
	{-1," 输出功率:",INTEGER,INT(Vadj.Power),INTEGER},
	{-1," 功率因素:",INTEGER,INT(Vadj.Factor),INTEGER},
	{-1," 电池电压:",INTEGER,INT(Vadj.BatV),INTEGER},
	{-1," 充电电流:",INTEGER,INT(Vadj.BatI),INTEGER},
	{-1," 充电状态:",INTEGER,INT(Vadj.ChargeStatus),INTEGER}
};

#define MMCNT 4
code MenuItem MainMenu[] = 
{
	{-1,"主菜单",MENU,0,MMCNT},
	{-1," 系统设置",MENU,SysSetting,0},
	{-1," 通讯设置",MENU,ComSetting,0},
	{-1," 参数校正",MENU,InfoAdj,0},
	{-1," 系统信息",MENU,Info,0},
	//{2,"退    出",FUNCTION,Exit,MMCNT},
};

#define FID(id)	(((id) - 1) / 3 * 3)
void ShowMenu(MENUPARAM *m)
{
	unsigned char fid = FID(m->id),i,x;
	if (!mr.inEdit) ClrScreen();
	cPrint12(0,0,0,(128-12*cStrLen((m->sm)->ItemStr))/2,(m->sm)->ItemStr,2);

	for(i =fid+1;i<=((fid+3<m->sm->mcnt)?(fid+3):(m->sm->mcnt));i++)
	{
		x = cPrint12(0,0,18+((i-1)%3)*16,0,(m->sm+i)->ItemStr,(((m->sm+i)->ItemType==MENU) && (m->id==i)));
		if ((m->sm+i)->mcnt==STRING)
		{
			if (CCHAR((m->sm+i)->ptr)>SettingArray[(m->sm+i)->id].len-1) CCHAR((m->sm+i)->ptr)=0;
			if (CCHAR((m->sm+i)->ptr)<0) CCHAR((m->sm+i)->ptr)=SettingArray[(m->sm+i)->id].len-1;
			//cPrint12(0,0,18+((i-1)%3)*16,x,*(SettingArray[(m->sm+i)->id].sa+CCHAR((m->sm+i)->ptr)),(m->id==i)?(mr.inEdit?0x81:1):0);
			Print(x, 18+((i-1)%3)*16,*(SettingArray[(m->sm+i)->id].sa+CCHAR((m->sm+i)->ptr)),(m->id==i)?(mr.inEdit?0xC0:0x41):0x40);
		}
		else if ((m->sm+i)->ItemType==CHAR) 
		{
			//PrintNum(0,0,18+((i-1)%3)*16,x,CCHAR((m->sm+i)->ptr),((m->sm+i)->mcnt!=CONST && (m->id==i))?(mr.inEdit?0x81:1):0);
			Print(x,18+((i-1)%3)*16,(char*)((m->sm+i)->ptr),((m->sm+i)->mcnt!=CONST && (m->id==i))?(mr.inEdit?0x88:0x09):0x08);
		}
		else if ((m->sm+i)->ItemType==INTEGER) 
		{
			//PrintNum(0,0,18+((i-1)%3)*16,x,CINT((m->sm+i)->ptr),((m->sm+i)->mcnt!=CONST && (m->id==i))?(mr.inEdit?0x91:0x11):0);
			Print(x,18+((i-1)%3)*16,(char*)((m->sm+i)->ptr),((m->sm+i)->mcnt!=CONST && (m->id==i))?(mr.inEdit?0x90:0x11):0x10);
		}
	}
}

void Welcome() //Welcome
{
	ClrScreen();

	FontMode(0,0);
	FontSet(0,1);
	cPrint12(1,0,1,24,"电力专用UPS",0);
	cPrint12(0,0,24,0,"上海统部电器有限公司",0);
	FontMode(1,0);
	FontSet(1,1);
	PutString(18,46,"TBU10KVA220V");
}

bit ReadSetting()
{
	return ModMst(2,3,0x4000,4,(unsigned char*)&SetValue);
}

bit SaveSetting()
{
	return ModMst(2,16,0x4000,4,(unsigned char*)&SetValue);
}

void InitMenu()
{
	LCD_Init();

	mr.inEdit = 0;
	mr.mm = MainMenu;
	mr.sm = mr.mm;
	mr.id = 1;
	mr.bid = mr.id;
	pMainMenu = MainMenu;
	
	//ReadSetting();

	//LightOn(127); 

	ShowMenu(&mr);
}

void Menu(unsigned char key)
{
	switch (key)
	{
	case K_UP: case K_UPOK: case K_UPESC:
	{
		if (mr.inEdit)
		{
			if ((mr.sm+mr.id)->ItemType == CHAR)
			{
				CCHAR((mr.sm+mr.id)->ptr)+=(key==K_UP)?1:10;;
			}
			else if ((mr.sm+mr.id)->ItemType == INTEGER)
			{
				CINT((mr.sm+mr.id)->ptr)+=(key==K_UP)?1:(key==K_UPOK)?10:100;
			}
		}
		else if ((mr.sm+mr.id)->mcnt == CONST)
		{
			if (mr.id>3) mr.id = (mr.id - 3) / 3 * 3 + 1; else mr.id = mr.sm->mcnt / 3 * 3 + 1;
		}
		else
		{
			if (mr.id > 1) mr.id --; else mr.id = (mr.sm->mcnt);
		}
		break;
	}
	case K_DN: case K_DNOK: case K_DNESC:
	{
		if (mr.inEdit)
		{
			if ((mr.sm+mr.id)->ItemType == CHAR)
			{
				CCHAR((mr.sm+mr.id)->ptr)-=(key==K_DN)?1:10;
			}
			else if ((mr.sm+mr.id)->ItemType == INTEGER)
			{
				CINT((mr.sm+mr.id)->ptr)-=(key==K_DN)?1:(key==K_DNOK)?10:100;
			}
		}
		else if ((mr.sm+mr.id)->mcnt == CONST)
		{
			if (mr.id<(mr.sm->mcnt)/3*3+1) mr.id = (mr.id + 3) / 3 * 3 + 1; else mr.id = 1;
		}
		else
		{
			if (mr.id < (mr.sm->mcnt)) mr.id ++; else mr.id = 1;
		}
		break;
	}
	case K_OK:
		if ((mr.sm+mr.id)->mcnt != CONST && ((mr.sm+mr.id)->ItemType == CHAR || (mr.sm+mr.id)->ItemType == INTEGER))
		{
			if (mr.inEdit)
			{
				mr.inEdit = 0;
				//SaveSetting();
				SettingChanged = 1;
				modscan = 1;
			}
			else
			{
				mr.inEdit = 1;
			}
		}
		else if ((mr.sm+mr.id)->ItemType == MENU)
		{
			mr.sm = CMENU((mr.sm+mr.id)->ptr);
			mr.bid = mr.id;
			mr.id = 1;
		}
		else if ((mr.sm+mr.id)->ItemType == FUNCTION)
		{
			CFUNC((mr.sm+mr.id)->ptr)();
		}
		break;
	case K_ESC:
		mr.inEdit = 0;
		//ReadSetting();
		modscan = 2;

		if (mr.sm == mr.mm)
		{
			Welcome();
			key = 0;
		}
		else
		{
			mr.sm = mr.mm;
			mr.id = mr.bid;
			mr.bid = 0;
		}
	}
	
	if (ModMst_Idle && modscan)
	{
		if (modscan==1)	//save setting
		{
			if (ModMst(2,16,0x4000,4,(unsigned char*)&SetValue)) 
				modscan = 0;
		}
		else if (modscan==2 || modscan == 3)	//read setting
		{
			if (modscan==3) SettingChanged = 1;
			if (ReadSetting()) 
			{
				modscan = 0;
			}
		}
	}

	if (key)
	{
		ShowMenu(&mr);
		//ShowChar(108,56,key,1);
	 	if (mr.sm==Info) DspCnt=100; else DspCnt = 0;
	}

	if (DspCnt>100)
	{
		ShowMenu(&mr);
		DspCnt = 1;
	}

	if (key)
	{
		
		LightOn(127);
	}

	if (!LightCnt && isBackLightOn)
	{
		LightOff();
	}
}

#endif //__MENUCFILE__