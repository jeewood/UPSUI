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
	unsigned char ModbusSA;
	unsigned char wMode;
	unsigned char sMode;
	unsigned char BKLtime;
	unsigned char BaudRate;
	unsigned char DataBits;
	unsigned char StopBits;
	unsigned char OddEven;
}SValue;

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
