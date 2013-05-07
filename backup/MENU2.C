#include "reg52.h"
#include "define.h"
#include "menu2.h"
#include "LCD_Dis.h"
#include "LCD_Dis.c"

#ifdef _KEY
#include "key.h"
#endif

CValue IValue={0};

unsigned char kState = 0;
unsigned char iItemID = 0,bItemID = 0;
unsigned int DspCnt = 0;

MENUPARAM mr = {0};

void (*MenuProc)(unsigned char ID, unsigned char key);

#define MICnt(m) (sizeof((m)) / sizeof(MenuItem))
#define FID(ID) ((((ID) / 3) * 3))

void MenuDrv(void)
{
	DspCnt++;
}

void LightOn(unsigned char Deg)
{
	SetBackLight(Deg);
}

void LightOff()
{
	SetBackLight(0);
}

code MenuItem SysSetting[]=
{
	{0,"工作方式:",CHAR,CHAR(IValue.Mode.BX.h),3},
	{1,"启动模式:",CHAR,CHAR(IValue.Mode.BX.l),3},
	{2,"背光时间:",CHAR,CHAR(IValue.Status.BX.h),3}
};

code MenuItem ComSetting[]=
{
	{0,"波特率:",CHAR,CHAR(IValue.ComSet[0].BX.h),4},
	{1,"字据位:",CHAR,CHAR(IValue.ComSet[0].BX.l),4},
	{2,"停止位:",CHAR,CHAR(IValue.ComSet[1].BX.h),4},
	{3,"校验位:",CHAR,CHAR(IValue.ComSet[1].BX.l),4}
};

code MenuItem Info[]=
{
	{0,"输入电压:",INTEGER,INT(IValue.ACInV),11},
	{1,"输入电流:",INTEGER,INT(IValue.ACInI),11},
	{2,"输入频率:",CHAR,CHAR(IValue.Freq.BX.h),11},
	{3,"逆变电压:",INTEGER,INT(IValue.InvV),11},
	{4,"负载电流:",INTEGER,INT(IValue.LoadI),11},
	{5,"逆变频率:",CHAR,CHAR(IValue.Freq.BX.l),11},
	{6,"输出功率:",INTEGER,INT(IValue.Power),11},
	{7,"功率因素:",INTEGER,INT(IValue.Factor),11},
	{8,"电池电压:",INTEGER,INT(IValue.BatV),11},
	{9,"充电电流:",INTEGER,INT(IValue.BatI),11},
	{10,"充电状态:",CHAR,CHAR(IValue.Status.BX.l),11}
};

code MenuItem MM[]=
{
	{0,"系统设置",SUBMENU,SysSetting,3},
	{1,"通讯设置",SUBMENU,ComSetting,3},
	{2,"系统信息",SUBMENU,Info,3}
};


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

void ShowMenu(MENUPARAM * m)
{
	#define ISCMD(idx,ID) (((m[idx].type)==SUBMENU) && ((m[idx].id)==(ID)))
	#define ISVAL(idx,ID) (((m[idx].type)!=SUBMENU) && ((m[idx].id)==(ID)))
	unsigned char x;

	if ((FID(m->id)+0) < (CMENU(m->ptr)+m->id)->mcnt)
	{
		x = cPrint12(0,0,18,0,(CMENU(m->ptr)+FID(m->id)+0)->ItemStr,ISCMD(FID(m->id)+0,m->id));
		if (m[FID(m->id)+0].type != SUBMENU)
		{
			cPrint12(0,0,18,x,CMENU(m->ptr)[FID(m->id)+0].ItemStr,ISVAL(FID(m->id)+0,m->id));
		}
	}
	if ((FID(m->id)+1) < CMENU(m->ptr)[m->id].mcnt)
	{
		x = cPrint12(0,0,34,0,CMENU(m->ptr)[FID(m->id)+1].ItemStr,ISCMD(FID(m->id)+1,m->id));
		if (m[FID(m->id)+1].type != SUBMENU)
		{
			cPrint12(0,0,34,x,CMENU(m->ptr)[FID(m->id)+1].ItemStr,ISVAL(FID(m->id)+1,m->id));
		}
	}
	if ((FID(m->id)+2) < CMENU(m->ptr)[m->id].mcnt)
	{
		x = cPrint12(0,0,50,0,CMENU(m->ptr)[FID(m->id)+2].ItemStr,ISCMD(FID(m->id)+2,m->id));
		if (m[FID(m->id)+2].type != SUBMENU)
		{
			cPrint12(0,0,50,x,CMENU(m->ptr)[FID(m->id)+2].ItemStr,ISVAL(FID(m->id)+2,m->id));
		}
	}
	//LCDChar(cID);
}

void InitMenu(void)
{
	LightOn(127);
	mr.inEdit = 0;
	mr.type = SUBMENU;
	mr.id = 0;
	mr.ptr = MENU(MM);
	Welcome();
}

void Menu(unsigned char key)
{
	if (key)
	{
		switch(key)
		{
			case K_UP:
				if (mr.inEdit)
				{
				}
				else if (mr.type != WELCOME)
				{
					if (mr.id<(CMENU(mr.ptr)+mr.id)->mcnt - 1) mr.id ++; else mr.id = 0;
				}
			break;
			case K_DN:
				if (mr.inEdit)
				{
				}
				else if (mr.type != WELCOME)
				{
					if (mr.id>0) mr.id --; else mr.id = (CMENU(mr.ptr)+mr.id)->mcnt - 1;
				}
			break;
			case K_OK:
				if (mr.inEdit)
				{
					mr.inEdit = 0;
					mr.ptr = mr.bptr;
					mr.bptr = 0;
				}
				else if (mr.type == WELCOME)
				{
					mr.inEdit = 0;
					mr.ptr = MENU(MM);
					mr.bptr = 0;
				}
				else
				{
					mr.inEdit = 1;
					mr.bptr = mr.ptr;
					mr.ptr = (CMENU(mr.ptr)+mr.id)->ptr;
				}
			break;
			case K_ESC:
				if (mr.inEdit)
				{
				}
				else if (mr.bptr == MM)
				{
					mr.ptr = mr.ptr;
					mr.bptr = 0;
				}
				else
				{
					mr.ptr = mr.bptr;
					mr.bptr = MM;
				}
			break;
			case K_AT:
			break;
			case K_OFF:
			break;
		}

		/*if (mr.type==FUNCTION)
		{
			CFUNC(mr.ptr)();
		}*/
		LCDChar(mr.id);
	}
}

void LCDChar(unsigned char key)
{
	FontMode(1,0);
	FontSet(0,1);
	ShowChar(0,0,key,1);
}

void Show_Menu(MENUPARAM* m)
{
	if (m->inEdit)
	{
	}
	else
	{
		if (m->type == SUBMENU)
		{
			
		}
	}
}

void Menu_Proc(unsigned char key)
{
	switch(key)
	{
		case K_UP:
		break;
		case K_DN:
		break;
		case K_OK:
		break;
		case K_ESC:
		break;
		case K_AT:
		break;
		case K_OFF:
		break;
	}
}
