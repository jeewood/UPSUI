#include "define.h"

#ifndef __LCD_CONFIG__
#define __LCD_CONFIG__

#define Dis_X_MAX		128-1
#define Dis_Y_MAX		64-1

//Define the MCU Register

sbit SPI_CS = P0^4;				//定义CS接在P1.0端口
sbit SPI_SDA = P0^5;			//定义SDA接在P1.1端口
sbit SPI_SCK = P0^6;			//定义SCK接在P1.2端口
sbit SPI_RES = P0^7;			//定义RESET接P1.3端口

#endif //__LCD_CONFIG__

