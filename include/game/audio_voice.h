#ifndef GAME_AUDIO_VOICE_H
#define GAME_AUDIO_VOICE_H

#include <stdbool.h>
#include "game/functions.h"

struct AdpcmLoopState;
struct RecordState {
    unsigned words[4];
    union {
        unsigned word;
        struct { unsigned char first, second, third, fourth; } bytes;
    } flags;
    unsigned value;
};
struct AdpcmCoefficients {
    short pairs[8][2];
};

struct AudioChannelState {
    unsigned char reserved00[0x1e];
    unsigned short flags1e;
    unsigned char reserved20[0x4c];
    unsigned short flags;
    unsigned char reserved6e[0x0e];
    struct AdpcmCoefficients coefficients;
};

struct AudioChannel {
    unsigned char reserved00[0x68];
    struct AudioChannelState *state;
};

struct RecordState;
struct AudioChannelOwner { unsigned char reserved00[0x18dc]; struct AudioChannel *channels[24]; };
struct AudioVoiceGroup {
    struct AudioChannel *channels[2];
    int count;
    void (*callback)(struct AudioVoiceGroup *group, int event, void *context);
    void *context;
    unsigned char active;
    unsigned char flag15;
    unsigned char flag16;
    unsigned char flag17;
    unsigned char reserved18[4];
    struct RecordState *record;
    unsigned short dirty;
    unsigned char reserved22;
    signed char mode23;
    unsigned char reserved24[8];
    unsigned char option2c;
    unsigned char option2d;
    unsigned char reserved2e[2];
    float value30;
    float value34;
    float value38;
    float value3c;
    unsigned reserved40;
    float value44;
    float sends[2];
    unsigned char reserved50[8];
    struct PoolLink link;
};

struct AudioEnvelope {
    unsigned char state;
    unsigned char reserved01[7];
    float attack;
    float release;
    float sustain;
    unsigned short decay;
    unsigned char reserved16[2];
    unsigned char hold;
};

struct AudioSlot {
    unsigned char reserved00[0x10];
    signed char mode;
    unsigned char state;
    unsigned char reserved12[2];
    struct AudioSlot *next;
    unsigned char reserved18[0x18];
};

struct AudioVector4 { float x, y, z, w; };
struct AudioWords4 { unsigned int words[4]; };
struct AudioByteRamp {
    unsigned char start;
    unsigned char end;
    unsigned short duration;
    unsigned short elapsed;
};

struct AudioVoice {
    struct AudioSlot slots[2];
    unsigned char reserved60[0x38];
    struct AudioEnvelope envelope;
    struct AudioVector4 vectorb4;
    unsigned char reservedc4[0x10];
    struct AudioVector4 vectord4;
    unsigned char reservede4[0x10];
    struct AudioVector4 vectorf4;
    unsigned char reserved104[0x10];
    struct AudioWords4 words114;
    unsigned char reserved124[0x10];
    unsigned char flags134[4];
    unsigned char flag138;
    unsigned char flag139;
    unsigned char flag13a;
    unsigned char flag13b;
    unsigned char flag13c;
    unsigned char reserved13d;
    signed char mode13e;
    unsigned char reserved13f;
    float value140;
    float value144;
    float value148;
    float value14c;
    float value150;
    float value154;
    float value158;
    float sends15c[2];
    float value164;
    float value168;
    int value16c;
    int value170;
    unsigned char reserved174[0x0c];
    struct AudioByteRamp ramp180;
    unsigned char reserved186[0x0a];
    int value190;
    unsigned char note194;
    unsigned char reserved195;
    unsigned char group196;
    unsigned char reserved197[5];
    float value19c;
    void (*callback)(struct AudioVoice *, int, void *);
    void *callback_data;
    struct AudioVoiceGroup *group;
    struct AudioVoice *next;
};

struct AudioControlBase {
    unsigned char reserved00[0x14];
    float value14;
    float value18;
    float value1c;
    float value20;
    float value24;
    float value28;
    signed char mode2c;
    unsigned char reserved2d[3];
    float value30;
    unsigned char option34;
    unsigned char option35;
    unsigned char reserved36[2];
    float sends[2];
    unsigned char reserved40[0x1c];
    unsigned char flag5c;
    unsigned char reserved5d[3];
    float value60;
};

struct AudioTrackControl;
struct AudioVoiceStartArgs;
struct AudioVoiceAllocator;
struct AudioVoiceAllocatorVTable {
    unsigned char reserved00[8];
    struct AudioVoice *(*allocate)(struct AudioVoiceAllocator *, struct AudioTrackControl *,
                                   unsigned char, const struct AudioVoiceStartArgs *);
};
struct AudioVoiceAllocator { const struct AudioVoiceAllocatorVTable *vtable; };
struct AudioTrackControl {
    struct AudioControlBase base;
    unsigned char reserved64[0x10];
    unsigned char transpose74;
    unsigned char reserved75[3];
    struct AudioByteRamp ramp78;
    unsigned char reserved7e[2];
    struct AudioVoiceAllocator *allocator;
};
struct RecordState;
struct TaskPool;
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00156398(struct AudioChannel *channel, struct RecordState *record);
extern void func_0017da08(struct AudioVoiceGroup *group, int index,
                          struct RecordState *record, bool remember);
extern struct AudioVoice *func_001d89ac(struct AudioTrackControl *parent,
    unsigned char index, const struct AudioVoiceStartArgs *arguments);
extern void func_0014e20c(struct AudioChannelState *state);
extern void func_001545ec(struct AudioChannel *channel, int mode);
extern void func_001545f4(struct AudioChannelState *state, int mode);
extern void func_00154608(struct AudioChannel *channel, unsigned int value);
extern void func_00154610(struct AudioChannelState *state, unsigned int value);
extern void func_001550e0(struct AudioChannel *channel, const struct AdpcmCoefficients *coefficients);
extern struct TaskPool *func_00185c58(void);
extern void func_0017e764(struct TaskPool *pool, struct AudioVoiceGroup *group);
extern void func_001550e8(struct AudioChannelState *state,
                         const struct AdpcmCoefficients *coefficients);
extern void func_001563a0(struct AudioChannelState *state, struct RecordState *record);
extern void func_001c5994(struct AudioChannel *channel, const struct AudioSlot *slot);
extern void func_001d49e8(struct AudioVoiceGroup *group, int index, const struct AudioSlot *slot);
#ifdef __cplusplus
}
#endif

#endif
