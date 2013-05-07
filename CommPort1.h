#ifndef __COM1HFILE__
#define __COM1HFILE__

extern void Init_COM1();
extern void COM1_Start_Sent();
extern void COM1_Rcv();

extern bit COM1_RcvCompleted;
extern xdata char COM1_PARITY;

extern xdata char COM1_rsBuf[];
//extern xdata char COM1_sBuf[];
extern xdata int COM1_RcvCount;
extern xdata int COM1_SndCount;

#endif //__COM1HFILE
