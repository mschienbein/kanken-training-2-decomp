#ifndef GAME_AUDIO_VOICE_H
#define GAME_AUDIO_VOICE_H

struct ObjectPool;
struct AnimationTrack;
struct AudioBiquadCoefficients;
struct HandleManager;
struct AudioCounter;
struct AudioFadeController;
struct AudioLinearRamp;
struct AudioSlotBufferDescriptor001d9580;
struct AudioVoiceOwner001d9580;
struct ByteCursor001db068;
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
    struct AudioSlot *pending;
    unsigned short value34;
    unsigned short value36;
    float value38;
    float value3c;
    float values40[10];
    float value68;
    unsigned short flags;
    unsigned char flag6e;
    unsigned char dirty;
    struct StreamMutex mutex;
    struct AdpcmCoefficients coefficients;
};

struct AudioChannel {
    unsigned char reserved00[4];
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
    unsigned char reserved24[0x40];
    float gain;
    struct AudioChannelState *state;
};

struct RecordState;
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
    unsigned char reserved19;
    unsigned char allocating;
    unsigned char allocation_failed;
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
    int priority;
    float value44;
    float sends[2];
    unsigned char format;
    unsigned char reserved51[7];
    struct PoolLink link;
};

/* [INFERENCE] Observed operation/entry prefixes; task_id is the full word
 * passed to cancellation, not an invented RegistryEntry pointer contract. */
struct SoundOperationEntry {
    unsigned char reserved00[8];
    struct AudioVoiceGroup *group;
    unsigned char reserved0c[0x3c8 - 0x0c];
};
struct SoundOperation {
    unsigned char reserved00[0x0d];
    unsigned char registered;
    unsigned char reserved0e[0x44 - 0x0e];
    struct RegistryEntry entry;
    unsigned char reserved50[0x10];
    unsigned task_id;
    unsigned char reserved64[0x220 - 0x64];
    unsigned count;
#ifndef __cplusplus
    struct SoundOperationEntry entries[];
#endif
};

struct AudioEnvelope {
    unsigned char state;
    unsigned char reserved01[3];
    float sweep;
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
    struct PoolLink link;
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
extern void func_001d9178(struct AudioVoice *voice, int increment);
extern void func_001d9530(struct AudioVoice *voice);
extern void func_001d7e6c(struct SoundOperation *operation);
/* [INFERENCE] Factory channel/priority words and callback/context are distinct
 * from the lower AudioVoiceGroup allocator's callback. The context remains a
 * pointer because the original delivered callback dereferences it. */
extern struct AudioVoice *func_001d5528(struct ObjectPool *pool);
extern struct AudioVoice *func_001d9198(int channels, int priority,
    void (*callback)(struct AudioVoice *, int, void *), void *context);
extern void func_001d96e8(struct AudioVoice *voice,
    void (*callback)(struct AudioVoice *, int, void *), void *context);
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
extern void func_001c599c(struct AudioChannelState *state, const struct AudioSlot *slot);
extern void func_001d49e8(struct AudioVoiceGroup *group, int index, const struct AudioSlot *slot);
extern const unsigned short data_0021dfcc[];
extern unsigned func_0014fe14(float value);
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
/* [INFERENCE] Separate copy of the two-byte cursor callback; this shares no
 * complete native CharStrmReader identity merely because its first word is a pointer. */
struct ShiftJisCursorState {
    const void *current;
#ifdef __cplusplus
    template<typename T> T get(int offset = 0) const
    {
        return static_cast<const T *>(current)[offset];
    }
    template<typename T> void step(int count = 1)
    {
        const T *&text = reinterpret_cast<const T *&>(current);
        text += count;
    }
#endif
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

/* c009-r5: one phase-in/value-out protocol for all five installed providers.
 * The retained allocation extent follows the initializer's 0x200-byte clear. */
struct GraphicsConverters {
    float (*wrap)(float phase);
    float (*curve)(float phase);
    float (*identity)(float phase);
    float (*step)(float phase);
    float (*random)(float phase);
    unsigned char reserved14[0x1ec];
};

#ifdef __cplusplus
extern "C" {
#endif
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
extern unsigned char func_001f2f2c(const struct AudioVoiceGroup *group);
extern unsigned func_001d4864(unsigned samples, int format);
extern void func_001835b8(struct AudioVoiceGroup *group);
extern unsigned func_001efcd0(const struct AudioChannel *channel);
extern void func_001d4850(struct ResetState001d4850 *state);
extern void func_001d0164(struct ResourceState001d0164 *state);
extern int func_00148a3c(struct ServiceCoordinator *context, unsigned value);
extern int func_001549d8(struct ServiceCoordinator *context);
extern float func_00149388(float phase);
extern void func_0014ee5c(struct AudioVoiceGroup *group);
extern void func_0014fc78(struct AudioChannel *channel, float volume);
extern float func_00151a1c(const struct AudioLinearRamp *ramp);
extern void func_00152610(struct AudioChannelState *state, unsigned char value);
extern void func_0014fbcc(struct AudioChannel *channel, int enabled);
extern void func_001525f0(struct AudioChannel *channel, int enabled);
extern void func_001d4a48(struct AudioVoiceGroup *group, unsigned mode);
extern void func_001d4aac(struct AudioVoiceGroup *group);
extern void func_0015274c(struct AudioChannel *channel, float gain);
extern void func_00150124(struct AudioVoiceGroup *group);
extern unsigned func_0014fe14(float value);
extern void func_0014fbec(struct AudioChannel *channel, unsigned frequency);
extern void func_0015263c(struct AudioChannel *channel,
                          const struct AudioBiquadCoefficients *parameters);
extern void func_001526b4(struct AudioChannel *channel, int mode);
extern void func_0014eec8(struct AudioVoiceGroup *group);
extern void func_0014f114(struct AudioVoiceGroup *group);
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
extern void func_0014fc98(struct AudioChannelState *state);
extern void func_001526b4(struct AudioChannel *channel, int mode);
extern void func_00155dec(struct AudioChannel *channel, int priority);
extern void func_00156484(struct HandleManager *manager,
                        struct AudioChannel *channel, int priority);
extern void func_001500bc(struct AudioVoiceGroup *group, int type, float factor);
extern void func_001c599c(struct AudioChannelState *state, const struct AudioSlot *notification);
/* [INFERENCE] The update consumes the same factory/callback AudioVoice receiver;
 * pinned Channel::Update(bool) corroborates, but does not recover its native name. */
extern void func_0014f304(struct AudioVoice *voice, bool update);
extern void func_0014d308(struct ObjectPool *pool);
extern unsigned int func_00155d24(unsigned int bits);
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
extern unsigned short func_001dafb0(struct ShiftJisCursorState *cursor);
extern void *func_001f15fc(const struct PackedPointer001f15fc *state);
extern unsigned func_001f2ef8(const struct AudioVoiceGroup *group);
extern void func_001d4adc(struct AudioFadeController *controller, bool muted,
                   unsigned int milliseconds);
#ifdef __cplusplus
}
#endif

#endif
