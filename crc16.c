#ifndef __CRC16CFILE__
#define __CRC16CFILE__

code unsigned int CRC16Table[] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

unsigned int crc16(unsigned char *buf, unsigned int len)
{
    xdata unsigned int CRC = 0xFFFF;
    xdata unsigned int  i;
    xdata unsigned char ch;

    for (i = 0; i < len; i++)
    {
        ch = *buf++;
        CRC = CRC16Table[(ch ^ CRC) & 15] ^ (CRC >> 4);
        CRC = CRC16Table[((ch >> 4) ^ CRC) & 15] ^ (CRC >> 4);
    }
    return CRC;//(CRC>>8) | (CRC<<8);
}

#endif
