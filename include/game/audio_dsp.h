#ifndef GAME_AUDIO_DSP_H
#define GAME_AUDIO_DSP_H

struct VoiceDspParameters;
struct ChannelDspParameters;
struct AudioDspReply;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
/* [INFERENCE] Source representation of the address-taken parameter body at
 * voice+0x04; route/gain retain body offsets+0x20/+0x24. */
struct VoiceDspPayload {
    float master_volume;
    unsigned char reserved08[8];
    unsigned short value10;
    unsigned char reserved12[4];
    unsigned short value16, value18, value1a, value1c, value1e;
    unsigned char reserved20[2];
    unsigned short value22;
    short route[2], gain[2];
};
struct VoiceDspParameters {
    unsigned flags;
    struct VoiceDspPayload parameters;
    unsigned char reserved2c[0xbc - 0x2c];
    unsigned short valuebc;
    unsigned char reservedbe[2];
    unsigned int flagsc0;
};

/* These are channel-table pointees, not the voice-table object above. */
struct ChannelDspParameters {
    unsigned flags;
    unsigned char reserved04[0x30];
    float value;
    unsigned char mode, option;
    short value3a, first, second;
    short coefficients[5];
    unsigned char reserved4a[0xa0 - 0x4a];
    unsigned char enabled, control_high;
    short count;
    unsigned char reserveda4[0xb0 - 0xa4];
    unsigned parameter;
    unsigned char reservedb4[8];
    unsigned short options, identifier;
};

struct AudioDspReply {
    unsigned short reserved00;
    unsigned short value;
};

#endif
