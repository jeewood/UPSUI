#ifndef __COM0HFILE__
#define __COM0HFILE__

extern void Init_COM0();
extern void Reinit_COM0();

extern void COM0_Start_Sent();
extern void COM0_Rcv();

extern bit COM0_RcvCompleted;
extern xdata char COM0_rsBuf[];
extern xdata int COM0_RcvCount;
extern xdata int COM0_SndCount;

#endif //__COM0HFILE__