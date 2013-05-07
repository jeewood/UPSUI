#if !defined(__MODSLVH__)
#define __MODSLVH__
extern unsigned char MM_InWaiting;
extern void ModSlv();
extern void MoveData(int * Src, char * Dst, unsigned char Len);
#endif