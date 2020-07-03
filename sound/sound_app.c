#include "sound.h"
#include "../include/stdio.h"

unsigned int volume = 80;
int play_sound(void)
{	
	struct wav_t wav;
	unsigned int soundAddr = 0x30100000 + sizeof(struct RIFF_HEADER) + sizeof(struct FMT_BLOCK) + sizeof(struct DATA_BLOCK) + 0x100000;
	/* 初始化codec控制引脚 */
	jz2440_init();
	
	/* 1.从nandflash中吧WAV读到sdram
	 * 2.获得WAV文件格式：通道数，采样率等
	 */
	read_wav(0x30100000, 0x200000, 0x2600000, &wav);	/* 不要拷贝到错误的地址，地址范围应该是_bss_end 到设置的栈之间，不覆盖栈 */
	
	printf("channels:%d\r\n", wav.channels);
	printf("bits_per_sample:%d\r\n", wav.bits_per_sample);
	printf("fs:%d\r\n", wav.fs);
	printf("wav_size:%#x\r\n", wav.wav_size);	
	
	if((wav.channels != 2) 
			|| ((wav.bits_per_sample !=8)&& (wav.bits_per_sample != 16)))
	{
		return -1;
	}
	/* 初始化IIS控制 */
	iis_init(wav.bits_per_sample, wav.fs);
	/* 初始化DMA */
	dma_init(soundAddr, wav.wav_size - 0x300000); /* 减3000去掉后面被添加的声音 */
	/* 初始化codec */
	wm8976_init();
	/* 设置音量 */
	set_volume(volume);
	//使能DMA
	//dma_start();
	//开启IIS传输
	//start_iis();
	//设置开关，音量
	//sound_set();

	return 0;

}
/* 调整声音大小 */
void sound_set(void)
{
	char c;
	printf("please enter +/- to change the volume or enter q/Q to quit loop\r\n");
	while(1)
	{
		c = getc();
		switch(c)
		{
			case 'Q':
			case 'q':
				{
						stop_iis();
						dma_stop();
						break;
				}
			case 's':
			case 'S':
				{
						start_iis();
						dma_start();
						break;
				}
			case '+':
				{
					if(volume <100)
						volume++;
					set_volume(volume);
					break;
			    }
			case '-':
				{
					if(volume > 0)
						volume--;
					set_volume(volume);
					break;
				}
			default:
				break;
		}
		printf("The current volume:\r\n");
		printf("volume:%d\r\n", volume);
	}

}