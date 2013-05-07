typedef union _INT
{
	unsigned int x;
	struct {
		char h;
		char l;
	}BX;	
}INT;

typedef struct
{
	int ACInV;
	int ACInI;
	int InvV;
	int LoadI;
	int BatV;
	int BusI;
	int Factor;
	int Power; //功率
	int LoadRate; //负载率

	INT Mode;//h Mode of Inv, l Mode of Start
	INT Status; //h BKL time
	INT Freq;// h ACInF, l InvF
	INT ComSet[2]; //0.h BandRate, 0.l DataBits; 1.h Stopbits, 1.l OddEven
}CValue;

typedef struct
{
	char bid; //MenuPage begin Idx;
	char eid; //MenuPage end Idx;
	char *ItemStr;
	char isSet;
	char (*f)(char kState);	//return State of Menu ID
}MenuItem;


extern void LightOn(unsigned char Deg);
extern void LightOff();
extern void InitMenu();
extern void ShowMenu(char mID);
extern void Menu(unsigned char Key);

