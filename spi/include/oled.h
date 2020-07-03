#ifndef __oled__
#define __oled__

extern void OLED_Init();
extern void OLED_Print(unsigned char * str, unsigned char page, unsigned char column);
extern void OledPutChar(unsigned char c,unsigned char page, unsigned char column);
extern void OLED_PrintHoriAddrMode(unsigned char * pstr);
extern void OLED_PrintHoriAddrModeMove(unsigned char * pstr);
#endif