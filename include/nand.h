#ifndef      _NAND_H
#define      _NAND_H

void nand_init(void);
void nand_read(unsigned char *buf, unsigned int start_addr, unsigned int size);
void nand_write(unsigned char *buf, unsigned int start_addr, unsigned int size);

#endif 

