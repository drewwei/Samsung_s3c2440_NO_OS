#ifndef         _WAV_H
#define         _WAV_H

#include "types.h"

struct RIFF_HEADER
{
    char szRiffID[4];  // 'R','I','F','F'
    DWORD dwRiffSize;
    char szRiffFormat[4]; // 'W','A','V','E'
};

struct WAVE_FORMAT
{
    WORD wFormatTag;
    WORD wChannels;
    DWORD dwSamplesPerSec;
    DWORD dwAvgBytesPerSec;
    WORD wBlockAlign;
    WORD wBitsPerSample;
};

struct FMT_BLOCK
{
    char  szFmtID[4]; // 'f','m','t',' '
    DWORD  dwFmtSize;
    struct WAVE_FORMAT wavFormat;
};

struct DATA_BLOCK
{
    char szDataID[4]; // 'd','a','t','a'
    DWORD dwDataSize;
};



struct wav_t {
    unsigned int fs;
    unsigned int channels;
    unsigned int bits_per_sample;
    unsigned int wav_size;
};


void read_wav(unsigned int sdramBase ,unsigned int wav_addr, unsigned int wav_size, struct wav_t *wav);
extern void nand_read(unsigned char *buf, unsigned int start_addr, unsigned int size);

#endif 


