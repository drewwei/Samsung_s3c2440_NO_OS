#ifndef      _NAND_H
#define      _NAND_H

extern void nand_read(unsigned char *buf, unsigned long start_addr, int size);
extern void nand_init(void);
extern void nand_write(unsigned char *buf, unsigned long start_addr, int size);

#endif // 

