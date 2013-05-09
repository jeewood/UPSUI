#if !defined(__MODMSTH__)
#define __MODMSTH__

extern bit ModMst_Idle;
extern char modscan;

extern void Init_ModMst();
extern bit ModMst(char SA, char FN, unsigned int DA, unsigned int DL, char * DataOrRTN);
extern void ModMst_Rcv();

#endif //__MODMSTH__