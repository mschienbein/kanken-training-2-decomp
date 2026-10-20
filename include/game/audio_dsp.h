#ifndef GAME_AUDIO_DSP_H
#define GAME_AUDIO_DSP_H

struct AudioDspParameterBlock;
struct AudioDspReply;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct AudioDspParameterBlock {
    unsigned int flags;
    float master_volume;
    unsigned char reserved08[8];
    unsigned short value10;
    unsigned char reserved12[4];
    unsigned short value16;
    unsigned short value18;
    unsigned short value1a;
    unsigned short value1c;
    unsigned short value1e;
    unsigned short reserved20;
    unsigned short value22;
    unsigned char reserved24[0x14];
    unsigned char mode38, option39;
    short value3a, first, second;
    short coefficients[5];
    unsigned char reserved4a[0x56];
    unsigned short valuea0;
    short count;
    unsigned char reserveda4[0x18];
    unsigned short valuebc;
    unsigned char reservedbe[2];
    unsigned int flagsc0;
};

struct AudioDspReply {
    unsigned short reserved00;
    unsigned short value;
};

#endif
