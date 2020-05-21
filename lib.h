#ifndef __LIB__
#define __LIB__



extern char *itoc(const int value);//整形转字符串
extern int strcmp(char *dest, char *src);
extern char*  strcpy(char *dest, char *src);
extern void * memcpy(void *dest, char *src, unsigned int len);
extern unsigned int strlen(char * str);
extern void memset(char *dest, int set, unsigned int len);
extern void * memcpy_int(void *dest, void *src, unsigned int len);
#endif