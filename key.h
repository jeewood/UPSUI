#ifndef __KEYHFILE__
#define __KEYHFILE__

#define K_UP  0x02
#define K_DN  0x10
#define K_OK  0x20
#define K_ESC 0x08
#define K_AT  0x01
#define K_OFF 0x04

#define K_UPOK K_UP+K_OK
#define K_DNOK K_DN+K_OK
#define K_UPESC K_UP+K_ESC
#define K_DNESC K_DN+K_ESC

extern void InitKeyDrv(void);
extern void keydrv(void);
extern unsigned char GetKey(void);

extern void Indicator(unsigned char mask);

#endif //__KEYHFILE__
