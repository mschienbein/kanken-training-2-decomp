#ifndef GAME_SOUND_RUNTIME_H
#define GAME_SOUND_RUNTIME_H

#include "game/pool_list.h"
#include "game/functions.h"

struct SoundInstance;
struct SoundHandle {
    struct SoundInstance *sound;
};

struct SoundInstanceVTable {
    unsigned char reserved00[0x10];
    void (*stop)(struct SoundInstance *sound);
};

struct SoundInstance {
    const struct SoundInstanceVTable *vtable;
    unsigned char reserved04[4];
    struct SoundHandle *handle;
    struct SoundHandle *secondary_handle;
    unsigned char reserved10[0x40];
    int priority_offset;
    unsigned char reserved54[0x49];
    unsigned char priority;
    unsigned char reserved9e[0x2a];
    float volume;
    unsigned char reservedcc[0x3c];
    struct PoolLink player_link;
    struct PoolLink selection_link;
};

struct SoundFileGroup {
    struct PoolList playing;
    struct PoolList pending;
    struct PoolList buffers;
    struct PoolList available;
    int requested;
    int limit;
    int buffer_count;
    float volume;
    float fade;
    int priority;
    float position[3];
    float velocity;
    unsigned char active;
};

struct SoundSelectionList {
    struct PoolList sounds;
    int limit;
};

struct SoundFileBuffer {
    const void *vtable;
    unsigned reserved04;
    struct SoundFileGroup *owner;
    unsigned char *begin;
    unsigned char *end;
    unsigned char *current;
    unsigned char active;
    unsigned char reserved19[3];
    struct PoolLink link;
};

struct SoundPlaybackState {
    const void *vtable;
    unsigned char reserved04[0x2c];
    float volume;
    float pitch;
    float values38[5];
    unsigned fields4c[3];
    float values58[2];
    float values60[4];
    unsigned fields70[2];
    float values78[2];
    unsigned fields80[2];
    float values88[2];
    unsigned fields90[2];
    unsigned char reserved98[7];
    unsigned char active;
    unsigned char reserveda0[0x18];
    float valuesb8[2];
    unsigned fieldsc0[2];
    unsigned char reservedc8[0x24];
    unsigned field_ec;
    unsigned field_f0;
    unsigned field_f4;
    struct PoolLink runtime_link;
    unsigned fields100[6];
};

struct SequenceSound {
    unsigned char reserved00[0x118];
    struct SoundPlaybackState playback;
};

struct WaveSound {
    unsigned char reserved00[0x128];
    struct SoundPlaybackState playback;
};

struct StreamSound {
    unsigned char reserved00[0x124];
    struct SoundPlaybackState playback;
};

struct SoundParameterTarget {
    unsigned char reserved00[0x83];
    unsigned char flag83;
    unsigned char reserved84[0x6c];
    unsigned char flagf0;
    unsigned char reservedf1[3];
    unsigned char flagf4;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned data_0022459c[];
extern const unsigned data_002245d8[];
extern void func_00190634(struct SoundInstance *sound);
extern void func_0019063c(struct SoundHandle *handle);
extern int func_001d2ce0(const struct SoundInstance *sound);
extern struct SoundInstance *func_001d4464(const struct SoundSelectionList *selection);
extern void func_001d2d14(struct SoundInstance *object, struct PoolList *registry);
#ifdef __cplusplus
}
#endif

#endif
