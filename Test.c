#include "modmst.h"
#include "modslv.h"

void IsTest(){
	ModMst(1,3,0,1,&Value[8]);
	COM1_rsBuf[0]=0x01;
	COM1_rsBuf[1]=0x03;
	COM1_rsBuf[2]=0x02;
	COM1_rsBuf[3]=0x27;
	COM1_rsBuf[4]=0x0F;
	COM1_rsBuf[5]=0xE3;
	COM1_rsBuf[6]=0xB0;
	COM1_RcvCount = 7;
	
	COM1_RcvCompleted = 1;
	
	ModMst_Rcv();
}
	
	