#include "wav.h"
#include "../../include/stdio.h"

//void read_wav(unsigned int wav_addr, int *fs, int *channels,int *bits_per_sample,int *wav_size)
/* 从nand地址为 wav_addr处读出wav数据到 sdramBase处*/
void read_wav(unsigned int sdramBase ,unsigned int wav_addr, unsigned int wav_size, struct wav_t *wav)
{
	struct FMT_BLOCK *fmtblk;
	struct DATA_BLOCK *datblk;
    char temp;
	nand_read((unsigned char *)sdramBase, wav_addr, wav_size);
	
	struct RIFF_HEADER *header = (struct RIFF_HEADER *)sdramBase;
    temp = header->szRiffID[4];
	header->szRiffID[4] = '\0';
    printf("szRiffID:%s\r\n", header->szRiffID);
	header->szRiffID[4] = temp;

	fmtblk = (struct FMT_BLOCK *) (sdramBase + sizeof(struct RIFF_HEADER));
    wav->bits_per_sample = fmtblk->wavFormat.wBitsPerSample;
	wav->channels		 = fmtblk->wavFormat.wChannels;
	wav->fs				 = fmtblk->wavFormat.dwSamplesPerSec;
	datblk			 	 = (struct DATA_BLOCK *)&fmtblk[1];
	wav->wav_size		 = datblk->dwDataSize;
}
























