#ifndef GAME_AUDIO_VOICE_H
#define GAME_AUDIO_VOICE_H

struct AnimationTrack;
struct AudioBiquadCoefficients;
struct AudioChannelArray;
struct AudioCounter;
struct AudioFadeController;
struct AudioLinearRamp;
struct AudioSlotBufferDescriptor001d9580;
struct AudioVoiceOwner001d9580;
struct AudioWorker;
struct ByteCursor001db068;
struct ConfigState001719f4;
struct ConfigTarget001719f4;
struct FloatState001d4ad4;
struct FloatState001d4b70;
struct FloatState001d4cec;
struct FloatState001d51b8;
struct FloatState001d5284;
struct FloatState001d5404;
struct HalfwordCursor001db054;
struct PackedPointer001f15fc;
struct PoolLink;
struct ResetState001d4850;
struct ResourceState001d0164;
struct SequenceEnabledState;
struct SequencePlayer;
struct ServiceCoordinator;
struct StreamCriticalSection;
struct StreamEvent;
struct StreamQueue;

#include "game/audio_filter.h"
#include "game/pool_list.h"
#include "game/stream_manager.h"

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
    unsigned int channel;
    unsigned short generation;
    unsigned short value06;
    unsigned int value08;
    unsigned char active;
    unsigned char mode;
    unsigned char flag0e;
    unsigned char flag0f;
    unsigned int value10;
    struct AudioBiquadCoefficients biquad;
    unsigned short flags1e;
    unsigned int sample_rate;
    float value24;
    unsigned char reserved28[4];
    unsigned int value2c;
    unsigned int value30;
    unsigned short value34;
    unsigned short value36;
    float value38;
    float value3c;
    float values40[10];
    float value68;
    unsigned short flags;
    unsigned char flag6e;
    unsigned char flag6f;
    unsigned char reserved70[0x0c];
    struct AdpcmCoefficients coefficients;
};;

struct AudioChannel {
    unsigned char reserved00[0x1c];
    float volume;
    unsigned char reserved20[0x44];
    float gain;
    struct AudioChannelState *state;
};;

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
    signed char flag17;
    unsigned char flag18;
    unsigned char reserved19[3];
    struct RecordState *record;
    unsigned short dirty;
    unsigned char reserved22;
    signed char mode23;
    float value24, value28;
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
    unsigned char format;
    unsigned char reserved51[7];
    struct PoolLink link;
};;

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
                          struct RecordState *record, int remember);
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


struct AudioCounter {
    unsigned long long value;
};

struct AudioLinearRamp {
    unsigned int reserved00;
    float start;
    float end;
    int duration;
    int elapsed;
};

struct AudioChannelArray {
    unsigned char reserved00[0x18dc];
    struct AudioChannel *channels[24];
};

struct SequenceEnabledState {
    unsigned char reserved00[0x34];
    unsigned char disabled;
};

struct AudioWorker {
    unsigned char reserved00[0x28];
    unsigned char stopping;
    unsigned char reserved29[0x0f];
    struct StreamQueue queue;
};

struct ConfigState001719f4 {
    unsigned char kind;
    unsigned char complex_format;
    unsigned char option;
    unsigned char reserved03;
    int index;
    unsigned char reserved08[0x10];
    struct ConfigTarget001719f4 *target;
};

struct ResourceState001d0164 {
    const void *vtable;
    unsigned field04;
    unsigned reserved08;
    unsigned char mask;
    unsigned char flags;
};

struct FloatState001d4ad4 {
    unsigned char reserved00[0x104];
    float value;
};

struct FloatState001d4b70 {
    unsigned char reserved00[0x18];
    float value;
};

struct FloatState001d4cec {
    unsigned char reserved00[0x14];
    float value;
};

struct FloatState001d51b8 {
    unsigned char reserved00[0x10];
    float value;
};

struct FloatState001d5284 {
    unsigned char reserved00[0x0c];
    float value;
};

struct FloatState001d5404 {
    unsigned char reserved00[8];
    float value;
};

struct AudioVoiceOwner001d9580 {
    unsigned char reserved00[0x0c];
    struct AudioVoice *voice;
};

struct AudioSlotBufferDescriptor001d9580 {
    const unsigned char *address;
    unsigned sample_count;
    unsigned char reserved08[9];
    unsigned char state;
    unsigned char reserved12[6];
};

struct HalfwordCursor001db054 {
    const unsigned short *current;
};

struct ByteCursor001db068 {
    const unsigned char *current;
};

struct PackedPointer001f15fc {
    unsigned char reserved00[0x2c];
    unsigned char selector;
    unsigned char reserved2d[7];
    unsigned char *address;
};

struct AudioFadeController {
    unsigned char reserved00[0x7b];
    bool muted;
    unsigned char reserved7c[0xb4];
    struct AudioVoice *voices;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct AudioCounter *func_00156dac(struct AudioCounter *counter);
extern unsigned int func_00180418(void);
extern void func_0015268c(struct AudioChannelState *state, float volume);
extern void func_00152758(struct AudioChannelState *state, float gain);
extern void func_0015510c(struct AudioChannelState *state);
extern void func_00155b24(struct AudioChannelState *state);
extern unsigned int data_00243748;
extern short *func_00169f9c(struct AnimationTrack *track, int index);
extern int func_001524ec(struct ServiceCoordinator *context, unsigned int channel);
extern int func_00152560(struct ServiceCoordinator *context, unsigned int channel);
extern void func_00145da8(struct StreamCriticalSection *critical);
extern void func_00145d3c(struct StreamCriticalSection *critical);
extern void func_00183900(struct StreamQueue *queue, struct StreamEvent *event);
extern int func_001dc390(int format);
extern void func_00171a34(struct ConfigTarget001719f4 *target, unsigned char option,
                          int index, unsigned flags);
extern unsigned char func_001f2f2c(const struct AudioVoiceGroup *group);
extern unsigned func_001d4864(unsigned samples, int format);
extern void func_001835b8(struct AudioVoiceGroup *group);
extern unsigned func_001efcd0(const struct AudioChannel *channel);
extern void func_001d4850(struct ResetState001d4850 *state);
extern void func_001d0164(struct ResourceState001d0164 *state);
extern int func_00148a3c(struct ServiceCoordinator *context, unsigned value);
extern int func_001549d8(struct ServiceCoordinator *context);
extern float func_00149388(void);
extern void func_0014ee5c(struct AudioVoiceGroup *group);
extern void func_0014fc78(struct AudioChannel *channel, float volume);
extern float func_00151a1c(const struct AudioLinearRamp *ramp);
extern void func_00152610(struct AudioChannelState *state, unsigned char value);
extern void func_0015274c(struct AudioChannel *channel, float gain);
extern void func_00154224(struct AudioChannelArray *array);
extern short *func_0016504c(struct SequencePlayer *player, int index);
extern void func_00168cd8(struct SequenceEnabledState *state, int enabled);
extern void func_001d88f0(struct SequencePlayer *player, int event, struct AnimationTrack *track);
extern void func_00148800(struct ServiceCoordinator *context, unsigned int value);
extern int func_00148854(struct ServiceCoordinator *context, unsigned int value);
extern int func_001488bc(struct ServiceCoordinator *context, unsigned int value);
extern int func_0014891c(struct ServiceCoordinator *context, unsigned int value);
extern int func_00148988(struct ServiceCoordinator *context, unsigned int value);
extern void func_001489e8(struct ServiceCoordinator *context, unsigned int value);
extern void func_001526e0(struct AudioChannelState *state, int mode);
extern unsigned int func_00155d24(unsigned int bits);
extern void func_001838ec(struct AudioWorker *worker);
extern void func_001719f4(struct ConfigState001719f4 *state, unsigned char kind, int format,
                  unsigned char option, int index, unsigned flags);
extern void func_001d4ad4(struct FloatState001d4ad4 *state, float value);
extern void func_001d4b70(struct FloatState001d4b70 *state, float value);
extern void func_001d4cec(struct FloatState001d4cec *state, float value);
extern void func_001d51b8(struct FloatState001d51b8 *state, float value);
extern void func_001d5284(struct FloatState001d5284 *state, float value);
extern void func_001d5404(struct FloatState001d5404 *state, float value);
extern void func_001d9580(struct AudioVoiceOwner001d9580 *owner,
                             const unsigned char *first, const unsigned char *last);
extern unsigned short func_001db054(struct HalfwordCursor001db054 *cursor);
extern unsigned char func_001db068(struct ByteCursor001db068 *cursor);
extern void *func_001f15fc(const struct PackedPointer001f15fc *state);
extern unsigned func_001f2ef8(const struct AudioVoiceGroup *group);
extern void func_001d4adc(struct AudioFadeController *controller, bool muted,
                   unsigned int milliseconds);
#ifdef __cplusplus
}
#endif

#endif
