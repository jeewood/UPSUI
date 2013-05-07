//Project added to Version control
#include "reg52.h"
#define _MODBUSMASTER_COM1
//#define _MODBUSMASTER_COM0
//#define _MODBUSSLAVE_COM1
#define _MODBUSSLAVE_COM0
//#define _COM0

#ifdef _MODBUSMASTER_COM0
	#define _COM0
	#define _MODBUSMASTER
#elif defined(_MODBUSMASTER_COM1)
	#define _COM1
	#define _MODBUSMASTER
#endif

#ifdef _MODBUSSLAVE_COM0
	#define _COM0
	#define _MODBUSSLAVE
#endif

#ifdef _COM0
	#define COM0_RSBUF_LEN 120
	#define COM0_BAUD 9600
	//extern register char COM0_PARITY;// 1 //0-NONE, 1-ODD, 2-EVEN
	#define COM0_RCVTIMEOUT 3
	#define COM0_TIMEOUT 300
#endif

#ifdef _COM1
	#define COM1_BAUD 9600
	//extern register char COM1_PARITY;// 0 //0-NONE, 1-ODD, 2-EVEN
	#define COM1_RCVTIMEOUT 5
	#define COM1_TIMEOUT 200

	#define COM1_RSBUF_LEN 120
#endif

//#define _WDT
#define _TIMER
//#define _PCA_TIMER
//#define _HSC
//#define _ADC
#define _BEEP
#define _FLASH
#define _PWM1
#define _KEY
//#define _LCD
#define _MENU


