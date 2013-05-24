#include "define.h"
#include "key.h"
#include "beep.h"

#ifndef __KEYCFILE__
#define __KEYCFILE__

sbit K1=P2^7;			//Key 1
sbit K2=P2^6;			//Key 2
sbit K3=P2^5;			//Key 3
sbit K4=P2^1;			//Key 4
sbit K5=P2^0;			//Key 5
sbit K6=P4^0;			//Key 6

sbit K1L=P4^4;			//Key 1 Indicator
sbit K2L=P4^5;			//Key 2 Indicator
sbit K3L=P4^1;			//Key 3 Indicator
sbit K4L=P2^2;			//Key 4 Indicator
sbit K5L=P2^3;			//Key 5 Indicator
sbit K6L=P2^4;			//Key 6 Indicator

xdata unsigned char keycode = 0;
xdata unsigned char keycode_bak = 0;
xdata unsigned int KeyCnt = 0;

void InitKeyDrv(void)
{
	P4SW = 0xFF;
}

void keydrv(void)
{
	//keycode_bak = keycode;
	keycode =0;
	if (K1) 		//Key 1
	{
		keycode |= 0x01;
		K1L = 0;
	}
	else
	{
		K1L = 1;
	}

	if (K2) 		//Key 2 
	{
		keycode |= 0x02;
		K2L = 0;
	}
	else
	{
		K2L = 1;
	}

	if (K3) 		//Key 3
	{
		keycode |= 0x04;
		K3L = 0;
	}
	else
	{
		K3L = 1;
	}

	if (K4) 		//Key 4
	{
		keycode |= 0x08;
		K4L = 0;
	}
	else
	{
		K4L = 1;
	}

	if (K5) 		//Key 5
	{
		keycode |= 0x10;
		K5L = 0;
	}
	else 
	{
		K5L = 1;
	}


	if (K6) 		//Key 6
	{
		keycode |= 0x20;
		K6L = 0;
	}
	else
	{
		K6L = 1;
	}
	
	if (KeyCnt) KeyCnt++; 
}

unsigned int KeyDly()
{
	return KeyCnt;
}

bit isKeyOn = 0;
unsigned char KeyPressed()
{
	return (keycode_bak!=0);
}

unsigned char GetKey(void)
{
	xdata unsigned char keys;
	if (keycode)
	{
		if (keycode != keycode_bak)
		{
			Beep(1);
			if (keycode_bak==K_UP || keycode_bak == K_DN)
			{
				keycode_bak = keycode;
				KeyCnt = 1;
				return keycode_bak;
			}
			keycode_bak = keycode;
			KeyCnt = 1;
		}
		else if (KeyCnt>300 && (KeyCnt % 10 == 0) && !isKeyOn)
		{
			Beep(1);
			if (keycode_bak==K_OFF) 
			{
				isKeyOn = 1;
				return K_ON;
			}
			return keycode_bak;
		}
	}
	else
	{
		if (!isKeyOn)
		{
			KeyCnt = 0;
			keys = keycode_bak;		
			keycode_bak = 0;
			return keys;
		}
		isKeyOn = 0;
		KeyCnt = 0;
		keycode_bak = 0;
	}
	return 0;
}

sbit LD2=P3^5;
sbit LD3=P3^2;
sbit LD4=P3^4;
sbit LD5=P3^3;

xdata long IndicatorCnt = 0;

void Indicator(unsigned char mask)
{
	if (mask & 0x01)
	{
		LD3 = 0;
	}
	else
	{
		LD3 = 1;
	}

	if (mask & 0x02)
	{
		LD4 = 1;
		if (IndicatorCnt>=100)
		{
			LD5 = ~LD5;
			IndicatorCnt = 0;
		}
		IndicatorCnt++;
	}
	else if (mask & 0x08)
	{
		LD5 = 1;
		if (IndicatorCnt>=100)
		{
			LD4 = ~LD4;
			IndicatorCnt = 0;
		}
		IndicatorCnt++;
	}
	else
	{
		LD4 = 1;
		LD5 = 1;
		IndicatorCnt = 0;
	}

	if (mask & 0x04)
	{
		LD2 = 0;
	}
	else
	{
		LD2 = 1;
	}	   

}

#endif //__KEYCFILE__

