#ifndef _DS18B20_H
#define _DS18B20_H
void ds18b20_init();

void ds18b20_send_byte(char c);

void ds18b20_send_bytes(char *p, unsigned int len);

void ds180b20_rev_bytes(char *buff, int count);

struct temp read_tempareture(void);


struct temp{
	int temp_l;
	int temp_h;
};

#endif