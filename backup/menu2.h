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
	int BatI;
	int Factor;
	int Power; //功率
	int LoadRate; //负载率

	INT Mode;//h Mode of Inv, l Mode of Start
	INT Status; //h BKL time,l Charge status
	INT Freq;// h ACInF, l InvF
	INT ComSet[2]; //0.h BandRate, 0.l DataBits; 1.h Stopbits, 1.l OddEven
}CValue;

typedef void (*FUNC)();
enum
{
	SUBMENU,
	CHAR,
	INTEGER,
	STRING,
	FUNCTION,
	WELCOME
};

typedef struct
{
	char id;
	char *ItemStr;
	char type;
	void *ptr;
	//void (*f)(unsigned char ID,char kState);	//return State of Menu ID
	char mcnt;
}MenuItem;

typedef struct
{
	char inEdit;
	char id;
	char type;
	void *ptr;
	void *bptr;
}MENUPARAM;

typedef struct
{
	char inEdit;
	MenuItem *mptr;
}MPTR;

#define CHAR(c) (&c)
#define INT(i) (&i)
#define STR(s) (s)
#define FUNC(f) (&f)
#define MENU(m)	(m)
#define VOID(p) ((void*)p)

#define CCHAR(c)	(*(char*)c)
#define CINT(i) 	(*(int*)i)
#define CSTR(s) 	((char*)s)
#define CFUNC(f) 	((FUNC)f)
#define CMENU(m) 	((MenuItem*)m)

extern void LightOn(unsigned char Deg);
extern void LightOff();

extern void MenuDrv(void);

extern void InitMenu();
//extern void ShowMenu(MENUPARAM *m);
extern void Menu(unsigned char key);
extern void LCDChar(unsigned char key);


