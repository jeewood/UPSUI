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

xdata CValue IValue = {0};
xdata CValue Vadj = {0};
xdata SValue SetValue = {0};

typedef struct _strArr
{
    int len;
    char **sa;
} strArr;

typedef struct _SettingModbus
{
	char f;
	int addr;
	int value;
} SMB;

enum ITEMTYPE
{
    CONST,      //0
    CHAR,		//1
    INTEGER,    //2
	STRING,		//3
    FUNCTION,   //4
	PASSWORD,   //5
    MENU        //6
};

enum MENUSTAT
{
	IDLE,
	BUSY,
	EDIT,
	PASS,
	COMM
};

#define TYPE(mit,misc) ((((unsigned char)(mit)<<5) & 0xE0) | ((unsigned char)(misc) & 0x1F))
#define MIT(type) (((unsigned char)(type)>>5) & 0x07)
#define MISC(misc) ((unsigned char)(misc) & 0x1F)

MENUPARAM mr = {0};
SMB smb = {0};

typedef void (*FUNC)();

#define CHAR(c)		(&c)
#define INT(i)      (&i)
#define STR(s)      (s)
#define FUNC(f)     (&f)

#define CCHAR(c)	(*(char*)(c))
#define CINT(i)     (*(int*)(i))
#define CSTR(s)     ((char*)(s))
#define CFUNC(f)    ((FUNC)(f))
#define CMENU(m)    ((MenuItem*)(m)) //(mr.ptr)+

#define cmi (mr.sm + mr.id)			//当前光标对应的菜单项

xdata int LightCnt = 0;
bit isBackLightOn = 0;
bit SettingChanged = 1;
int Password = 0;

xdata unsigned int DspCnt = 0;
void MenuDrv(void)
{
    if (DspCnt > 0)DspCnt++;
    if (LightCnt > 0) LightCnt--;
}

void LightOn(unsigned char Deg)
{
    SetBackLight(Deg);
    isBackLightOn = 1;
	if (SetValue.BKLtime>32767) 
	{
		LightCnt = -1;
		SetValue.BKLtime = -1;
	}
	else
	if (SetValue.BKLtime>=300)
	{
		SetValue.BKLtime = 300;
	}
   	LightCnt = SetValue.BKLtime * 100;
}

void LightOff() //关闭背光
{
    SetBackLight(0);
    isBackLightOn = 0;
}


void Welcome() //Welcome
{
    ClrScreen();

    FontMode(0, 0);
    FontSet(0, 1);
    cPrint12(1, 0, 1, 24, "电力专用UPS", 0);
    cPrint12(0, 0, 24, 0, "上海统部电器有限公司", 0);
    FontMode(1, 0);
    FontSet(1, 1);
    PutString(18, 46, "TBU10KVA220V");
}

code char *WorkMode[] = {"在线式", "离线式"};
code char *StartMode[] = {"手动", "自动"};

code char *BaudRate[] = {"1200", "2400", "4800", "9600"};
code char *DataBits[] = {"8", "7"};
code char *StopBits[] = {"1", "2"};
code char *OddEven[] = {"无", "奇", "偶"};

code char *StartCtrl[] = {"停止","启动"};
code char *ByPassCtrl[] = {"旁路","逆变"};

code strArr SettingArray[] =
{
    {2, WorkMode},      //0
    {2, StartMode},     //1
    {4, BaudRate},      //2
    {2, DataBits},      //3
    {2, StopBits},      //4
    {3, OddEven},       //5
	{2, StartCtrl},		//6
	{2, ByPassCtrl}		//7
};

code char *FaultInfo[] =
{
    {"交流失电"},
    {"旁路失电"},
    {"电池过压"},
    {"电池欠压"},
    {"旁路失电"},
    {"旁路失电"}
};

extern code MenuItem MainMenu[];

#define SSCNT 4
code MenuItem SysSetting[] =
{
    { -1, "系统设置", TYPE(MENU,SSCNT), MainMenu},
    {0, " 工作方式:", TYPE(STRING,INTEGER), INT(SetValue.wMode)},
    {1, " 启动模式:", TYPE(STRING,INTEGER), INT(SetValue.sMode)},
    { -1, " 背光时间:", TYPE(INTEGER,INTEGER), INT(SetValue.BKLtime)},
    { -1, " 设置密码:", TYPE(INTEGER,INTEGER), INT(SetValue.Password)}
};

#define CSCNT 5
code MenuItem ComSetting[] =
{
    { -1, "通迅设置", TYPE(MENU,CSCNT), MainMenu},
    { -1, " 站  址:", TYPE(CHAR,INTEGER), INT(SetValue.ModbusSA)},
    {2, " 波特率:", TYPE(STRING,INTEGER), INT(SetValue.BaudRate)},
    {3, " 数据位:", TYPE(STRING,INTEGER), INT(SetValue.DataBits)},
    {4, " 停止位:", TYPE(STRING,INTEGER), INT(SetValue.StopBits)},
    {5, " 校验位:", TYPE(STRING,INTEGER), INT(SetValue.OddEven)}
};

#define IMCNT 11
code MenuItem Info[] =
{
    { -1, "系统信息", TYPE(MENU,IMCNT), MainMenu},
    { -1, " 输入电压:", TYPE(CONST,INTEGER), INT(IValue.ACInV)},
    { -1, " 输入电流:", TYPE(CONST,INTEGER), INT(IValue.ACInI)},
    { -1, " 输入频率:", TYPE(CONST,INTEGER), INT(IValue.ACInF)},
    { -1, " 输出电压:", TYPE(CONST,INTEGER), INT(IValue.InvV)},
    { -1, " 负载电流:", TYPE(CONST,INTEGER), INT(IValue.LoadI)},
    { -1, " 输出频率:", TYPE(CONST,INTEGER), INT(IValue.InvF)},
    { -1, " 输出功率:", TYPE(CONST,INTEGER), INT(IValue.Power)},
    { -1, " 功率因素:", TYPE(CONST,INTEGER), INT(IValue.Factor)},
    { -1, " 电池电压:", TYPE(CONST,INTEGER), INT(IValue.BatV)},
    { -1, " 充电电流:", TYPE(CONST,INTEGER), INT(IValue.BatI)},
    { -1, " 充电状态:", TYPE(CONST,INTEGER), INT(IValue.ChargeStatus)}
};

#define IACNT 11
code MenuItem InfoAdj[] =
{
    { -1, "参数校正", TYPE(MENU,IACNT), MainMenu},
    { -1, " 输入电压:", TYPE(INTEGER,INTEGER), INT(Vadj.ACInV)},
    { -1, " 输入电流:", TYPE(INTEGER,INTEGER), INT(Vadj.ACInI)},
    { -1, " 输入频率:", TYPE(INTEGER,INTEGER), INT(Vadj.ACInF)},
    { -1, " 输出电压:", TYPE(INTEGER,INTEGER), INT(Vadj.InvV)},
    { -1, " 负载电流:", TYPE(INTEGER,INTEGER), INT(Vadj.LoadI)},
    { -1, " 输出频率:", TYPE(INTEGER,INTEGER), INT(Vadj.InvF)},
    { -1, " 输出功率:", TYPE(INTEGER,INTEGER), INT(Vadj.Power)},
    { -1, " 功率因素:", TYPE(INTEGER,INTEGER), INT(Vadj.Factor)},
    { -1, " 电池电压:", TYPE(INTEGER,INTEGER), INT(Vadj.BatV)},
    { -1, " 充电电流:", TYPE(INTEGER,INTEGER), INT(Vadj.BatI)},
    { -1, " 充电状态:", TYPE(INTEGER,INTEGER), INT(Vadj.ChargeStatus)}
};

#define PMCNT 1
code MenuItem PasswordMenu[] =
{
    { -1, "请输入设置密码", TYPE(MENU,PMCNT), MainMenu},
    { -1, " 密码:", TYPE(INTEGER,INTEGER), INT(Password)},
    //{2,"退    出",FUNCTION,Exit,MMCNT},
};

#define CMCNT 2
code MenuItem ControlMenu[] =
{
    { -1, "系统控制", TYPE(MENU,CMCNT), MainMenu},
    { 6, " 启停控制:", TYPE(STRING,INTEGER), INT(SetValue.StartCtrl)},
    { 7, " 手工旁路:", TYPE(STRING,INTEGER), INT(SetValue.ByPassCtrl)}
};

#define MMCNT 5
code MenuItem MainMenu[] =
{
    { -1, "主菜单", TYPE(FUNCTION,MMCNT), Welcome},
    { -1, " 系统信息", TYPE(MENU,0), Info},
    { -1, " 系统控制", TYPE(MENU,0), ControlMenu},
    { -1, " 系统设置", TYPE(PASSWORD,0), SysSetting},
    { -1, " 通讯设置", TYPE(PASSWORD,0), ComSetting},
    { -1, " 参数校正", TYPE(PASSWORD,0), InfoAdj}
};

//********************************************************
//i 控制标志
// 0000 0000
// 1 		 = InEdit								0x80
//  1 		 = cstring								0x40
//   1 		 = string								0x20
//    1 	 = Integer								0x10
//													
//      1 	 = char									0x08
//       1   = Font size 1=>Large font,0 small font	0x04
//        0  = NC									0x02
//         1 = invert								0x01
//********************************************************
void ShowMenu(MENUPARAM *m)
{
    #define FID(id) (((id) - 1) / 3 * 3)
    #define dcmi (m->sm + i)

    unsigned char fid = FID(m->id), i, x;
    if (!mr.stat) ClrScreen();
    cPrint12(0, 0, 0, (128 - 12 * cStrLen((m->sm)->ItemStr)) / 2, (m->sm)->ItemStr, 2);
    for (i = fid + 1; i <= ((fid + 3 < MISC(m->sm->ItemType)) ? (fid + 3) : MISC(m->sm->ItemType)); i++)
    {
        x = cPrint12(0, 0, 18 + ((i - 1) % 3) * 16, 0, dcmi->ItemStr, 
			((MIT(dcmi->ItemType) == MENU || MIT(dcmi->ItemType) == PASSWORD) && (m->id == i)));
        if (MIT(dcmi->ItemType) == STRING)
        {
            if (CINT(dcmi->ptr) > SettingArray[dcmi->id].len - 1) CINT(dcmi->ptr) = 0;
            if (CINT(dcmi->ptr) < 0) CINT(dcmi->ptr) = SettingArray[dcmi->id].len - 1;
            Print(x, 18 + ((i - 1) % 3) * 16, *(SettingArray[dcmi->id].sa + CINT(dcmi->ptr)),
				(m->id == i) ? (mr.stat==EDIT ? 0xC0 : 0x41) : 0x40);
        }
        else if (MIT(dcmi->ItemType) == CHAR)
        {
            Print(x, 18 + ((i - 1) % 3) * 16, ((char *)(dcmi->ptr)+1), 
                (m->id == i) ? (mr.stat==EDIT ? 0x88 : 0x09) : 0x08);
		}
        else if (MIT(dcmi->ItemType) == INTEGER)
        {
            Print(x, 18 + ((i - 1) % 3) * 16, (char *)(dcmi->ptr), 
				(m->id == i) ? (mr.stat==EDIT ? 0x90 : 0x11) : 0x10);
        }
        else if (MIT(dcmi->ItemType) == CONST)
        {
            Print(x, 18 + ((i - 1) % 3) * 16, (char *)(dcmi->ptr), 0x10);
        }
    }
}


bit ReadSetting()
{
    return ModMst(2, 3, 0x4000, 4, (unsigned char *)&SetValue);
}

void InitMenu()
{
    LCD_Init();
    mr.stat = IDLE;
    mr.mm = 0;
    mr.sm = MainMenu;
    mr.id = 1;
    mr.bid = mr.id;
    ShowMenu(&mr);
}

// mr.sm + mr.id => 当前项
// cmi->ptr => 为当前项 对应的 变量或子菜单 的指针

void Menu(unsigned char key)
{
    switch (key)
    {
    case K_UP: case K_UPOK: case K_UPESC:
    {
        if (mr.stat==BUSY)			//菜单忙,可能是不在菜单中,如调用FUNCTION
        {
            key = 0;
        }
        else if (mr.stat==EDIT)		//菜单项在编辑中
        {
            if (MISC(cmi->ItemType) == CHAR)
            {
                CCHAR(cmi->ptr) += (key == K_UP) ? 1 : 10;;
            }
            else if (MISC(cmi->ItemType) == INTEGER)
            {
                CINT(cmi->ptr) += (key == K_UP) ? 1 : (key == K_UPOK) ? 10 : 100;
            }
        }
        else 						//菜单项选择
		{
			if (MIT(cmi->ItemType) == CONST)	//菜单为仅显示,非设置项
	        {
	            if (mr.id > 3) mr.id = (mr.id - 3) / 3 * 3 + 1; else mr.id = MISC(mr.sm->ItemType) / 3 * 3 + 1;
	        }
	        else
	        {
	            if (mr.id > 1) mr.id --; else mr.id = MISC(mr.sm->ItemType);
	        }
		}
        break;
    }
    case K_DN: case K_DNOK: case K_DNESC:
    {
        if (mr.stat==BUSY)
        {
            key = 0;
        }
        else if (mr.stat==EDIT)
        {
            if (MISC(cmi->ItemType) == CHAR)
            {
                CCHAR(cmi->ptr) -= (key == K_DN) ? 1 : 10;
            }
            else if (MISC(cmi->ItemType) == INTEGER)
            {
                CINT(cmi->ptr) -= (key == K_DN) ? 1 : (key == K_DNOK) ? 10 : 100;
            }
        }
        else
		{
			if (MIT(cmi->ItemType) == CONST)
	        {
	            if (mr.id < (MISC(mr.sm->ItemType)) / 3 * 3 + 1) mr.id = (mr.id + 3) / 3 * 3 + 1; else mr.id = 1;
	        }
	        else
	        {
	            if (mr.id < (MISC(mr.sm->ItemType))) mr.id ++; else mr.id = 1;
	        }
		}
        break;
    }
    case K_OK:
    {
        if (mr.stat==BUSY)	//退出FUNCTION
        {
            mr.stat = IDLE;
        }
		else if (MIT(cmi->ItemType) == PASSWORD)	//需密码才能进入
		{
			mr.mm = CMENU(cmi->ptr);
			mr.sm = PasswordMenu;
			mr.bid = mr.id;                      //保存上级菜单的位置
			mr.id = 1;                           

            if (CINT(cmi->ptr) == SetValue.Password || CINT(cmi->ptr) == 1234)   //设置密码为1000
            {
                mr.sm = mr.mm;                  //进入设置(mr.mm指向要进入的设置菜单指针)
	            if (mr.sm == InfoAdj)           //是参数调整菜单?
	            {
	                jmemcpy((char *)&IValue, (char *)&Vadj, 24);   //复制显示值到设置项中
	            }
                mr.mm = 0;                      //已进入设置
            }
		}
        else if (MIT(cmi->ItemType) == MENU)
        {
            mr.sm = CMENU(cmi->ptr);
            mr.bid = mr.id;
            mr.id = 1;

        }
        else if (MIT(cmi->ItemType) == FUNCTION)
        {
            CFUNC(cmi->ptr)();
        }
		else if ((MIT(cmi->ItemType) == CHAR) || (MIT(cmi->ItemType) == INTEGER) || (MIT(cmi->ItemType) == STRING))
        {
            if (mr.stat==EDIT)//在编辑中,马上退出编辑状态
            {
				if (mr.mm != 0)	//是密码对话框?
				{
					if (CINT(cmi->ptr) == SetValue.Password || CINT(cmi->ptr) == 1234)
					{
						mr.sm = mr.mm;
			            if (mr.mm == InfoAdj)
			            {
			                jmemcpy((char *)&IValue, (char *)&Vadj, 24);
			            }
					}
					else		//密码不正确,返回原来的菜单
					{
						mr.sm = mr.sm->ptr;
						mr.id = mr.bid;
						mr.bid = 0;
					}
					mr.mm = 0;
				}
				else		//非密码对话框,即正常菜单
				{						
					//此处开始保存编辑的值
					if (mr.sm == InfoAdj)	//当前实现的是INTEGER类型数据的保存(MISC()==INTEGER)
					{
						ModMst(2,6,(cmi->ptr - &Vadj)/2,CINT(cmi->ptr),0);						
					}
					else
					{
						SettingChanged = 1;
						ModMst(2,6,0x4000+(cmi->ptr - &SetValue)/2,CINT(cmi->ptr),0);
					}
				}
				mr.stat = IDLE;
            }
            else	//不在编辑中,则进入编辑状态
            {
                mr.stat = EDIT;
            }
        }
        break;
    }
    case K_ESC:
    {
        if (mr.stat == EDIT)
        {
            mr.stat = IDLE;
        }

		if (MIT(mr.sm->ItemType) == FUNCTION)
        {
            CFUNC(mr.sm->ptr)(); 
			mr.stat = BUSY;
            Password = 0;            
            key = 0;
        }
		else
        {
            mr.sm = CMENU(mr.sm->ptr);
            mr.id = mr.bid;
            mr.bid = 0;
        }
        break;
    }
	default:
		key = 0;
    }

	//零时性的处理变量类型为整数,显示类型为字符的变量
	if (MIT(cmi->ItemType)==CHAR && MISC(cmi->ItemType)==INTEGER)
	{
		CINT(cmi->ptr) = CINT(cmi->ptr) & 0xFF;
	}


    if (key)
    {
        ShowMenu(&mr);
        //ShowChar(108,56,SetValue.ModbusSA>>8,1);

        LightOn(127);
        if (mr.sm == Info) DspCnt = 1; else DspCnt = 0;
    }

    if (DspCnt > 100)
    {
        ShowMenu(&mr);
        DspCnt = 1;
    }

    if (LightCnt==0 && isBackLightOn)
    {
        LightOff();
    }
}

#endif //__MENUCFILE__