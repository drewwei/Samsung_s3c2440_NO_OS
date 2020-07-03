#include "../../libmad/mad.h"
void mp3_decode()
{
    struct mad_decoder pDecoder;
    mad_decoder_run(&pDecoder, MAD_DECODER_MODE_SYNC);


}