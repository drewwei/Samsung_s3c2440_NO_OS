
#ifndef _STDIO_H
#define _STDIO_H


#define __DEBUG

#ifdef  __DEBUG
	#define DEBUG(format, ...) 		printf(format, ##__VA_ARGS__)
#else
	#define DEBUG(format, args...)		while(0)
#endif

#include "types.h"

#ifndef _VALIST
#define _VALIST
typedef char *va_list;
#endif /* _VALIST */

extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
extern int snprintf(char * buf, size_t size, const char *fmt, ...);
extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int sprintf(char * buf, const char *fmt, ...);
extern int vsscanf(const char * buf, const char * fmt, va_list args);
extern int sscanf(const char * buf, const char * fmt, ...);

extern void putc(unsigned char c);
extern unsigned char getc(void);

extern int printf(const char *fmt, ...);
extern int scanf(const char * fmt, ...);


extern int atoi(char *str);//字符串转数字 


#endif /* _STDIO_H */
