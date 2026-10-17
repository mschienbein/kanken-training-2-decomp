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
    unsigned char reserved1e[4];
    unsigned short value22;
    unsigned char reserved24[0x7c];
    unsigned short valuea0;
    unsigned char reserveda2[0x1a];
    unsigned short valuebc;
    unsigned char reservedbe[2];
    unsigned int flagsc0;
};

struct AudioDspReply {
    unsigned short reserved00;
    unsigned short value;
};

#endif
