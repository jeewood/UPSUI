#include "define.h"

#ifndef __BEEPCFILE__
#define __BEEPCFILE__

sbit BEEP = P3^7;

xdata unsigned int beepDelayCnt = 0,bDelayTime = 0;
bit isSound;

void InitBeep(void)
{
	P3M1 = 0x00;
	P3M0 |= 0x80;
	BEEP = 0;
}

void BeepDrv(void)
{
	if (beepDelayCnt>bDelayTime)
	{
		isSound = !isSound;
		BEEP = isSound;
		beepDelayCnt = 0;
	}
	if (bDelayTime)
	{
		beepDelayCnt = beepDelayCnt + 1;
	}
	else
	{
		BEEP = (isSound = 0);
	}
}

void Sound(unsigned int bDelay)
{
	bDelayTime = bDelay;
}

void Beep(unsigned char OnOff)
{
	BEEP = (OnOff!=0);
}

#endif //__BEEPCFILE__

