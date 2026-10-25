#ifndef GAME_AUDIO_DSP_H
#define GAME_AUDIO_DSP_H

struct AudioDspParameterBlock;
struct AudioDspReply;
struct ServiceCoordinator;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct AudioDspVolumeSettings {
    float master_volume;
    float output_volume[2];
};

/* [INFERENCE] The contiguous settings subobject begins at parent +4.
 * Existing field labels retain their offsets relative to the parent block. */
struct AudioDspSettings {
    struct AudioDspVolumeSettings volume;
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

struct AudioDspParameterBlock {
    unsigned int flags;
    struct AudioDspSettings settings;
};

struct AudioDspReply {
    unsigned short reserved00;
    unsigned short value;
};

#ifdef __cplusplus
extern "C" {
#endif
extern int func_00155ab4(struct ServiceCoordinator *coordinator, int channel,
    short count);
extern int func_0014cc1c(struct ServiceCoordinator *coordinator, int channel,
    int mode, unsigned char option);
extern int func_0014ce8c(struct ServiceCoordinator *coordinator, int channel,
    short b0, short b1, short b2, short a1, short a2);
#ifdef __cplusplus
}
#endif

#endif
