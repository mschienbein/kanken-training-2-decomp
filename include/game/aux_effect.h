#ifndef GAME_AUX_EFFECT_H
#define GAME_AUX_EFFECT_H

struct AudioRecord12;
struct ChannelWorkspace;
struct AuxEffectNode;
struct AuxEffectSlot;
struct AuxEffectState;
struct AuxEffectVTable;
struct AuxFloatRamp;
struct AuxFrameState;
struct ExtendedWorkspace;
struct InputLatchSnapshot;
struct PoolLink;
struct PoolList;
struct ServiceOutput;
struct SoundSubsystem;

#include "game/pool_list.h"
#include "game/audio_voice.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
/* func_001543b8 supplies four planar buffer pointers and a scalar sample count.
 * [INFERENCE] Sample signedness and count signedness are not fixed by that call. */
typedef void (*AuxCallback)(int **channels, unsigned sample_count, void *context);

struct AuxFloatRamp {
    float start;
    float target;
    int duration;
    int elapsed;
};

struct AuxEffectState {
    void *buffer;
    unsigned int reserved04;
    unsigned int size;
    unsigned char active;
    signed char channel;
    unsigned char enabled;
    unsigned char frame;
};

struct AuxEffectSlot {
    struct AuxEffectState *effect;
    unsigned char state;
    unsigned char reserved05[3];
};

struct SoundSubsystem {
    struct AudioLinearRamp master_volume;
    struct AuxFloatRamp volume[2];
    unsigned char reserved34[0x20];
    struct PoolList effects[2];
    AuxCallback saved_callbacks[2];
    const void *tables[128];
    void *saved_contexts[2];
    unsigned int reserved27c;
    unsigned long long ticks[2];
    struct AuxEffectSlot slots[2][2];
};

struct AudioRecord12 {
    unsigned int word00;
    unsigned int word04;
    unsigned char active;
};

struct AuxEffectNode {
    const struct AuxEffectVTable *vtable;
    struct PoolLink link;
};

struct AuxFrameState {
    unsigned char reserved00[0x1c];
    unsigned char active;
    unsigned char frame;
};

struct AudioReadiness {
    unsigned char reserved00[0x0e];
    unsigned char ready, generation;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0018362c(struct SoundSubsystem *manager, unsigned char channel);
extern void func_00183614(signed char channel, AuxCallback callback, void *context);
extern void func_0014b154(struct ServiceOutput *manager, float volume);
extern unsigned char func_0014b174(const struct ServiceOutput *state);
extern void func_0014c930(struct ServiceOutput *manager, signed char channel);
extern void func_0014c990(struct ServiceOutput *manager, signed char channel);
extern void func_0017e654(struct AuxFloatRamp *ramp, int duration, float target);
extern void func_0017e848(int index, float value);
extern int func_001525c8(signed char channel, struct ChannelWorkspace *workspace);
extern int func_001525dc(signed char channel, struct ExtendedWorkspace *workspace);
extern void func_00152c88(int **channels, unsigned sample_count, void *context);
extern struct AuxFrameState bss_004b6584;
extern int func_0014c90c(struct InputLatchSnapshot *flags, int kind, int channel);
extern void func_00146150(struct ServiceOutput *manager, signed char channel,
                          AuxCallback callback, void *context);
extern void func_00148358(signed char channel, AuxCallback *callback, void **context);
extern void func_00148370(float volume);
extern unsigned char func_001487f4(void);
extern void func_0014b078(signed char channel);
extern void func_0014b234(signed char channel);
extern void *func_0014ee30(void *storage);
extern int func_001504a8(struct SoundSubsystem *manager, unsigned char channel,
                  struct ChannelWorkspace *workspace);
extern int func_0015050c(struct SoundSubsystem *manager, unsigned char channel,
                  struct ExtendedWorkspace *workspace);
extern int func_00150570(struct SoundSubsystem *manager, unsigned char channel,
                  struct AuxEffectNode *effect);
extern int func_00151d50(struct AuxEffectState *effect, int enabled);
#ifdef __cplusplus
}
#endif

#endif
