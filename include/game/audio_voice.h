#ifndef GAME_AUDIO_VOICE_H
#define GAME_AUDIO_VOICE_H

struct AnimationTrack;
struct AudioChannel;
struct AudioVoice;
struct AudioBiquadCoefficients;
struct HandleManager;
struct AudioCounter;
struct AudioFadeController;
struct AudioLinearRamp;
struct AudioVoiceOwner001d9580;
struct ConfigState001719f4;
struct ConfigTarget001719f4;
struct FloatState001d4ad4;
struct FloatState001d4b70;
struct FloatState001d4cec;
struct FloatState001d51b8;
struct FloatState001d5284;
struct FloatState001d5404;
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
#include "game/voice_wave_info.h"

#include <stdbool.h>
#include "game/functions.h"
#ifdef __cplusplus
#include <preinclude.h>
#include "nw/snd/snd_VoiceManager.h"
#include "nw/snd/snd_CurveAdshr.h"
#endif

/* S03: the four status bytes and the next-buffer pointer are separate fields. */
struct RecordState {
    const void *data;
    unsigned sample_count;
    /* 00153bd8 stores the actual destination of ADPCM loop-state copy. */
    const struct AdpcmLoopState *loop_context;
    unsigned sample_limit;
    /* Original 001d914c sign-extends this byte; other reads use truncation. */
    signed char loop;
    unsigned char status;
    unsigned short generation;
    struct RecordState *next;
#ifdef __cplusplus
    RecordState();
#endif
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
    struct RecordState *pending;
    short value34;
    short value36;
    float value38;
    float value3c;
    float values40[10];
    float value68;
    unsigned short flags;
    unsigned char flag6e;
    unsigned char flag6f;
    struct StreamMutex mutex;
    struct AdpcmCoefficients coefficients;
};

#ifdef __cplusplus
extern "C" {
    typedef void AudioChannelCallback(struct AudioChannel*, nw::snd::internal::driver::Voice*);
}
struct AudioChannel {
    unsigned index;
    unsigned char mode;
    unsigned char alignment;
    unsigned char reserved06;
    unsigned char flags;
    unsigned char reserved08[4];
    struct AudioBiquadCoefficients filter;
    unsigned char reserved16[2];
    int extent;
    float volume;
    int priority;
    struct AudioChannel *previous, *next;
    AudioChannelCallback *callback;
    nw::snd::internal::driver::Voice *callback_context;
    float mix_values[12];
    float gain;
    struct AudioChannelState *state;
};
#endif




struct AudioByteRamp {
    unsigned char start;
    unsigned char end;
    unsigned short duration;
    unsigned short elapsed;
};

struct AudioModulationState {
    unsigned char reserved00[0x10];
    unsigned elapsed;
    float value;
    float scale;
    unsigned char active;
    unsigned char pending;
    unsigned char reserved1e[2];
};

#ifdef __cplusplus
extern "C" {
    typedef void AudioVoiceCallback(struct AudioVoice*, int, void*);
}
struct AudioVoice {
    AudioVoice();
    struct RecordState wave_buffers[2][2];
    unsigned char reserved60[0x38];
    nw::snd::internal::CurveAdshr envelope;
    struct AudioModulationState modulation[4];
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
    unsigned char mode197;
    unsigned char reserved198[4];
    float value19c;
    AudioVoiceCallback *callback;
    void *callback_data;
    nw::snd::internal::driver::Voice *group;
    struct AudioVoice *next;
    struct PoolLink pool_link;
};
#endif

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
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00156398(struct AudioChannel *channel, struct RecordState *record);
#ifdef __cplusplus
extern void func_0017da08(nw::snd::internal::driver::Voice *group, int index,
                          struct RecordState *record, int remember);
#endif
extern struct AudioVoice *func_001d89ac(struct AudioTrackControl *parent,
    unsigned char index, const struct AudioVoiceStartArgs *arguments);
extern void func_0014e20c(struct AudioChannelState *state);
extern void func_001545ec(struct AudioChannel *channel, int mode);
extern void func_001545f4(struct AudioChannelState *state, int mode);
extern void func_00154608(struct AudioChannel *channel, unsigned int value);
extern void func_00154610(struct AudioChannelState *state, unsigned int value);
extern void func_001550e0(struct AudioChannel *channel, const struct AdpcmCoefficients *coefficients);
#ifdef __cplusplus
extern nw::snd::internal::driver::VoiceManager *func_00185c58(void);
extern void func_0017e764(nw::snd::internal::driver::VoiceManager *pool,
                         nw::snd::internal::driver::Voice *group);
#endif
extern void func_001550e8(struct AudioChannelState *state,
                         const struct AdpcmCoefficients *coefficients);
extern void func_001563a0(struct AudioChannelState *state, struct RecordState *record);
extern void func_001c5994(struct AudioChannel *channel, const struct RecordState *slot);
extern void func_001c599c(struct AudioChannelState *state, const struct RecordState *slot);
#ifdef __cplusplus
extern void func_001d48d4(nw::snd::internal::driver::Voice *voice, const struct VoiceWaveInfo *info, unsigned sample_offset);
extern void func_001d9238(struct AudioVoice *voice, const struct VoiceWaveInfo *info, unsigned sample_offset);
extern void func_001d49e8(nw::snd::internal::driver::Voice *group, int index,
                          const struct RecordState *slot);
#endif
#ifdef __cplusplus
}
#endif


struct AudioCounter {
    unsigned long long value;
};

struct AudioLinearRamp {
    unsigned char active, output_mode;
    unsigned char reserved02[2];
    float start;
    float end;
    int duration;
    int elapsed;
};


struct SequenceEnabledState {
    unsigned char reserved00[0x34];
    unsigned char disabled;
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
/* [INFERENCE] Converter role names. The original 0017e6c8 dispatcher supplies
 * its phase in s0 and consumes the float result for the selected slot,
 * including the identity and uncached random cases. The trailing storage is
 * retained from the initializer's observed 0x200-byte clear. */
struct GraphicsConverters {
    float (*wrap)(float phase);
    float (*curve)(float phase);
    float (*identity)(float phase);
    float (*step)(float phase);
    float (*random)(float phase);
    unsigned char reserved14[0x1ec];
};

extern struct GraphicsConverters bss_004b8440;
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
#ifdef __cplusplus
extern unsigned char func_001f2f2c(const nw::snd::internal::driver::Voice *group);
#endif
extern unsigned func_001d4864(unsigned samples, int format);
#ifdef __cplusplus
extern void func_001835b8(nw::snd::internal::driver::Voice *group);
#endif
extern unsigned func_001efcd0(const struct AudioChannel *channel);
extern void func_001d4850(struct ResetState001d4850 *state);
extern void func_001d0164(struct ResourceState001d0164 *state);
extern int func_00148a3c(struct ServiceCoordinator *context, unsigned value);
extern int func_001549d8(struct ServiceCoordinator *context);
extern float func_00149388(float phase);
#ifdef __cplusplus
extern void func_0014ee5c(nw::snd::internal::driver::Voice *group);
#endif
extern void func_0014fc78(struct AudioChannel *channel, float volume);
extern float func_00151a1c(const struct AudioLinearRamp *ramp);
extern void func_00152610(struct AudioChannelState *state, unsigned char value);
extern void func_0014fbcc(struct AudioChannel *channel, int enabled);
extern void func_001525f0(struct AudioChannel *channel, int enabled);
#ifdef __cplusplus
extern void func_001d4a48(nw::snd::internal::driver::Voice *group, unsigned mode);
extern void func_001d4aac(nw::snd::internal::driver::Voice *group);
#endif
extern void func_0015274c(struct AudioChannel *channel, float gain);
extern void func_00154224(struct HandleManager *array);
extern void func_0014603c(struct HandleManager *owner);
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
extern void *func_001f15fc(const struct PackedPointer001f15fc *state);
#ifdef __cplusplus
extern unsigned func_001f2ef8(const nw::snd::internal::driver::Voice *group);
#endif
extern void func_001d4adc(struct AudioFadeController *controller, bool muted,
                   unsigned int milliseconds);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
void func_00150030(nw::snd::internal::driver::Voice *group, float value);
void func_0015643c(nw::snd::internal::driver::Voice *group);
struct AudioChannel *func_00153fec(int priority,
    void (*callback)(struct AudioChannel *, nw::snd::internal::driver::Voice *),
    nw::snd::internal::driver::Voice *group);
int func_0015477c(nw::snd::internal::driver::VoiceManager *pool, int priority);
void func_001d94a0(struct AudioVoice *voice, const struct VoiceWaveInfo *info,
                   unsigned id, unsigned sample_offset);
void func_00150098(nw::snd::internal::driver::Voice *group, float value);
void func_001501ec(nw::snd::internal::driver::Voice *group, float value);
void func_0017e6a4(nw::snd::internal::driver::Voice *group, float value);
void func_00150210(nw::snd::internal::driver::Voice *group, int index, float value);
void func_00154624(struct AudioChannel *element, nw::snd::internal::driver::Voice *group);
void func_0017e47c(nw::snd::internal::driver::Voice *group, unsigned int value);
void func_0017e4bc(nw::snd::internal::driver::Voice *group, int mode);
void func_001d49a8(nw::snd::internal::driver::Voice *group, unsigned char value);
void func_001d49c8(nw::snd::internal::driver::Voice *group, unsigned char value);
void func_001d9178(struct AudioVoice *voice, int increment);
void func_001d9530(struct AudioVoice *voice);
void func_0015263c(struct AudioChannel *channel,
                   const struct AudioBiquadCoefficients *coefficients);
void func_001551fc(struct AudioVoice *voice, float value, int mode);
void func_001d911c(struct AudioVoice *voice, signed char mode);
void func_001d9220(struct AudioVoice *voice, float value, int duration, unsigned char enabled);
void func_001c5950(struct AudioChannel *channel, unsigned mode);
void func_001c5970(struct AudioChannel *channel, unsigned direction);
void func_001c59fc(struct AudioChannel *channel, unsigned alignment);
void func_0017e470(nw::snd::internal::driver::Voice *group, unsigned int index,
                  const struct AdpcmCoefficients *source);
}
#endif

#ifdef __cplusplus
extern "C" {
extern nw::snd::internal::driver::Voice *func_00138cb0(nw::snd::internal::driver::Voice *resource);
extern void func_00152f10(nw::snd::internal::driver::Voice *resource, int event, void *context);
extern void func_0018290c(nw::snd::internal::driver::Voice *resource, int event, void *context);
extern void func_00183550(nw::snd::internal::driver::Voice *group);
extern void func_0016a27c(nw::snd::internal::driver::Voice *resource, int enabled);
extern struct AudioVoice *func_001d5528(struct ObjectPool *pool);
extern struct AudioVoice *func_001d9198(int channels, int priority,
    void (*callback)(struct AudioVoice *, int, void *), void *context);
extern void func_001d96e8(struct AudioVoice *voice,
                         void (*callback)(struct AudioVoice *, int, void *), void *context);
extern nw::snd::internal::driver::Voice *func_0017e500(nw::snd::internal::driver::VoiceManager *pool, int channels, int priority,
                                     void (*callback)(nw::snd::internal::driver::Voice *, int, void *),
                                     void *context);
extern struct AudioChannel *func_00154838(struct HandleManager *pool,
    int priority, void (*callback)(struct AudioChannel *, nw::snd::internal::driver::Voice *),
    nw::snd::internal::driver::Voice *group);
extern void func_00155d68(nw::snd::internal::driver::VoiceManager *pool, nw::snd::internal::driver::Voice *group);
extern void func_00155dec(struct AudioChannel *channel, int priority);
}
#endif

#endif
