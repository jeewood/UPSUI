#ifndef __MENU__
#define __MENU__

typedef union _INT
{
	unsigned short x;
	struct {
		char h;
		char l;
	}BX;	
}INTX;

typedef struct
{
	int ACInV;
	int ACInI;
	int InvV;
	int LoadI;
	int BatV;
	int BatI;
	int Factor;
	int Power; //功率
	int LoadRate; //负载率
	int ACInF;
	int InvF;
	int ChargeStatus;
}CValue;

typedef struct
{
	unsigned int ModbusSA;	//0
	unsigned int wMode;		//1
	unsigned int sMode;		//2	
	unsigned int BKLtime;	//3		
	unsigned int Password;	//4		
	unsigned int BaudRate;	//5		
	unsigned int DataBits;	//6		
	unsigned int StopBits;	//7		
	unsigned int OddEven;	//8		
	unsigned int StartCtrl;	//9		
	unsigned int ByPassCtrl;//10			
}SValue;

typedef struct _MenuItem
{
    char id;
    char *ItemStr;
    char ItemType;  
	//高3位用于显示时ptr对应的类型
    //｛CHAR,INTEGER为数字,STRING为预定义的字符串｝
    //低5位当高3位为MENU时,低5位为菜单项数(最多31个)
    //		 高3位不是MENU时,高3位为变量类型,低5位为本项显示类型
    void *ptr;
    //char mcnt;
} MenuItem;

typedef struct
{
    char stat;	//状态
    char id;
    char bid; //backup id

    MenuItem *mm;
    MenuItem *sm;
} MENUPARAM;

extern MENUPARAM mr;

extern xdata SValue SetValue;
extern xdata CValue IValue;
extern xdata CValue Vadj;
extern bit SettingChanged;

extern void MenuDrv(void);
extern void LightOn(unsigned char Deg);
extern void LightOff();
extern void InitMenu();
extern bit ReadSetting();
extern void Menu(unsigned char key);

#endif
