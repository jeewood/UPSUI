#include "reg52.h"
#include "define.h"
#include "menu1.h"
#include "LCD_Dis.h"
#include "LCD_Dis.c"

#ifdef _KEY
#include "key.h"
#endif

CValue IValue={0};

unsigned char kState = 0;
unsigned char iItemID = 0,bItemID = 0;
unsigned int DspCnt = 0;

/*MENUPARAM mr = {0};
MPTR *mprev ={0};
MPTR *mcurr ={0};
MPTR *mnext ={0};
*/
void (*MenuProc)(unsigned char, unsigned char);

#define MICnt(m) (sizeof((m)) / sizeof(MenuItem))
#define FID(ID) ((((ID) / 3) * 3))

void MainMenu(unsigned char key);

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

void Welcome(unsigned char key) //Welcome
{
	switch(key)
	{
		case K_OK: 
			MenuProc = MainMenu;
			iItemID = 0;
		break;

		case K_ESC:
			MenuProc = Welcome;
			iItemID = 0;
		break;
	}

	if (MenuProc==Welcome)
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
	else
	{
		MenuProc(0);
	}
}

code MenuItem SysSetting[]=
{
	{0,"工作方式:",INTEGER,0,0},
	{1,"启动模式:",INTEGER,0,0},
	{2,"背光时间:",INTEGER,0,0}
};

void SysSettingProc(char key)
{
	switch(key)
	{								    
		case K_UP: 
			if (iItemID>0) iItemID--; else iItemID = MICnt(SysSetting)-1;
		break;

		case K_DN:
			if (iItemID<MICnt(SysSetting)-1)iItemID++; else iItemID = 0; 
		break;

		case K_OK: 
			//MenuProc = SysSetting[iItemID].f;
			//iItemID = 0;
		break;

		case K_ESC: 
			MenuProc = MainMenu;
			iItemID = bItemID;
			bItemID = 0;
		break;
	}

	if (MenuProc==SysSettingProc)
	{
		ClrScreen();
		cPrint12(1,1,0,30,"系统设置",0);
		ShowMenu(SysSetting,MICnt(SysSetting),iItemID);	
	}
	else
	{
		MenuProc(0);
	}
}

code MenuItem ComSetting[]=
{
	{0,"波特率:",INTEGER,0,0},
	{1,"字据位:",INTEGER,0,0},
	{2,"停止位:",INTEGER,0,0},
	{3,"校验位:",INTEGER,0,0}
};

void ComSettingProc(char key)
{
	switch(key)
	{
		case K_UP: 
			if (iItemID>0) iItemID--; else iItemID = MICnt(ComSetting)-1;
		break;

		case K_DN:
			if (iItemID<MICnt(ComSetting)-1)iItemID++; else iItemID = 0; 
		break;

		case K_OK: 
			//MenuProc = ComSetting[iItemID].f;
			//iItemID = 0;
		break;

		case K_ESC: 
			MenuProc = MainMenu;
			iItemID = bItemID;
			bItemID = 0;
		break;
	}

	if (MenuProc==ComSettingProc)
	{
		ClrScreen();
		cPrint12(1,1,0,30,"通讯设置",0);
		ShowMenu(ComSetting,MICnt(ComSetting),iItemID);	
	}
	else
	{
		MenuProc(0);
	}
}

code MenuItem Info[]=
{
	{0,"输入电压:",INTEGER,0,0},
	{1,"输入电流:",INTEGER,0,0},
	{2,"输入频率:",INTEGER,0,0},
	{3,"逆变电压:",INTEGER,0,0},
	{4,"负载电流:",INTEGER,0,0},
	{5,"逆变频率:",INTEGER,0,0},
	{6,"输出功率:",INTEGER,0,0},
	{7,"功率因素:",INTEGER,0,0},
	{8,"电池电压:",INTEGER,0,0},
	{9,"充电电流:",INTEGER,0,0},
	{10,"充电状态:",INTEGER,0,0}
};

void InfoProc(char key)
{
	switch(key)
	{
		case K_UP: 
			if (iItemID>=3) iItemID = FID(iItemID-3); else iItemID = FID(MICnt(Info)-1);
		break;

		case K_DN:
			if (iItemID<MICnt(Info)-4)iItemID=FID(iItemID+3); else iItemID = 0; 
		break;

		case K_ESC: 
			MenuProc = MainMenu;
			iItemID = bItemID;
			bItemID = 0;
		break;
	}

	if (MenuProc==InfoProc)
	{
		ClrScreen();
		cPrint12(1,1,0,30,"运行信息",0);
		ShowMenu(Info,MICnt(Info),iItemID);
	}
	else
	{
		MenuProc(0);
	}
}

code MenuItem MM[]=
{
	{0,"系统设置",SUBMENU,SysSetting,SysSettingProc},
	{1,"通讯设置",SUBMENU,ComSetting,ComSettingProc},
	{2,"系统信息",SUBMENU,Info,InfoProc}
};

void ShowMenu(MenuItem* m,unsigned char mcnt, unsigned char cID)
{
	#define ISCMD(idx,ID) (((m[idx].type)==SUBMENU) && ((m[idx].id)==(ID)))
	#define ISVAL(idx,ID) (((m[idx].type)!=SUBMENU) && ((m[idx].id)==(ID)))
	unsigned char x;

	if ((FID(cID)+0) < mcnt)
	{
		x = cPrint12(0,0,18,0,m[FID(cID)+0].ItemStr,ISCMD(FID(cID)+0,cID));
		if (m[FID(cID)+0].type != SUBMENU)
		{
			cPrint12(0,0,18,x,m[FID(cID)+0].ItemStr,ISVAL(FID(cID)+0,cID));
		}
	}
	if ((FID(cID)+1) < mcnt)
	{
		x = cPrint12(0,0,34,0,m[FID(cID)+1].ItemStr,ISCMD(FID(cID)+1,cID));
		if (m[FID(cID)+1].type != SUBMENU)
		{
			cPrint12(0,0,34,x,m[FID(cID)+1].ItemStr,ISVAL(FID(cID)+1,cID));
		}
	}
	if ((FID(cID)+2) < mcnt)
	{
		x = cPrint12(0,0,50,0,m[FID(cID)+2].ItemStr,ISCMD(FID(cID)+2,cID));
		if (m[FID(cID)+2].type != SUBMENU)
		{
			cPrint12(0,0,50,x,m[FID(cID)+2].ItemStr,ISVAL(FID(cID)+2,cID));
		}
	}
	//LCDChar(cID);
}

void MainMenu(unsigned char key)
{
	switch(key)
	{
		case K_UP: 
			if (iItemID>0) iItemID--; else iItemID = MICnt(MM)-1;
		break;

		case K_DN:
			if (iItemID<MICnt(MM)-1)iItemID++; else iItemID = 0; 
		break;

		case K_OK: 
			MenuProc = MM[iItemID].f;
			bItemID = iItemID;
			iItemID = 0;
		break;

		case K_ESC:
			MenuProc = Welcome;
			iItemID = 0;
		break;
	}

	if (MenuProc==MainMenu)
	{
		ClrScreen();
		cPrint12(1,1,0,30,"主菜单",0);
		ShowMenu(MM,MICnt(MM),iItemID); 
	}
	else
	{
		MenuProc(0);
	}
}

void InitMenu(void)
{
	LightOn(127);
	MenuProc = Welcome;
}

void Menu(unsigned char key)
{
	if (key)
	{
		MenuProc(key);
		DspCnt = 0;
		//MenuProc(0);
	}
	else if (DspCnt>100)
	{
		DspCnt = 0;
		MenuProc(0);
	}
}

void LCDChar(unsigned char key)
{
	FontMode(1,0);
	FontSet(0,1);
	ShowChar(0,0,key,1);
}
/*
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
*/