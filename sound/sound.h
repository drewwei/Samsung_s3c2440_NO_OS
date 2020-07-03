#ifndef	 _SOUND_H
#define	_SOUND_H

#include "codec/wm8976.h"
#include "format/wav.h"
#include "machine/jz2440.h"
#include "soc/dma.h"
#include "soc/iis.h"

int play_sound(void);

void sound_set(void);



#endif