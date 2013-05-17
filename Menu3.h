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

/*
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
	unsigned int RatedCurr; //11
}SValue;
*/

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

//extern xdata SValue SetValue;
//extern xdata CValue IValue;
//extern xdata CValue Vadj;
extern xdata INTX sValue[];
#define Adj_ACInV          sValue[0].x
#define Adj_ACInI          sValue[1].x
#define Adj_InvV           sValue[2].x
#define Adj_LoadI          sValue[3].x
#define Adj_BatV           sValue[4].x
#define Adj_BatI           sValue[5].x
#define Adj_Factor         sValue[6].x
#define Adj_Power          sValue[7].x
#define Adj_LoadRate       sValue[8].x
#define Adj_ACInF          sValue[9].x
#define Adj_InvF           sValue[10].x
#define Adj_ChargeStatus   sValue[11].x

#define ModbusSA           sValue[12].x
#define wMode              sValue[13].x
#define sMode              sValue[14].x
#define BKLtime            sValue[15].x
#define Password           sValue[16].x
#define BaudRate           sValue[17].x
#define DataBits           sValue[18].x
#define StopBits           sValue[19].x
#define OddEven            sValue[20].x
#define StartCtrl          sValue[21].x
#define ByPassCtrl         sValue[22].x
#define RatedCurr          sValue[23].x

extern xdata INTX Value[];
#define ACInV              Value[0].x
#define ACInI              Value[1].x
#define InvV               Value[2].x
#define LoadI              Value[3].x
#define BatV               Value[4].x
#define BatI               Value[5].x
#define Factor             Value[6].x
#define Power              Value[7].x
#define LoadRate           Value[8].x
#define ACInF              Value[9].x
#define InvF               Value[10].x
#define ChargeStatus       Value[11].x

extern bit SettingChanged;

extern void MenuDrv(void);
extern void LightOn(unsigned char Deg);
extern void LightOff();
extern void InitMenu();
extern bit ReadSetting();
extern void Menu(unsigned char key);

#endif
