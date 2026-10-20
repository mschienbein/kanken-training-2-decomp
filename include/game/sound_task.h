#ifndef GAME_SOUND_TASK_H
#define GAME_SOUND_TASK_H

struct EmbeddedSoundInstance;
struct PoolLink;
struct SignalState;
struct SoundByteRamp;
struct SoundPlaybackState;
struct SoundPlayerBase;
struct SoundPoolStorage;
struct SoundRegularInstance;
union SynchronizationStorage;
struct SoundShortBufferDescriptor;
struct SoundSmallDescriptor;
struct SoundStreamPlayer;
struct SoundSubPlayer;
struct SoundTaskBase;
struct SoundTaskRegistryEntry;
struct StreamSemaphore;
struct TimingState;

#include "game/functions.h"
#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/sound_runtime.h"
#include "game/stream_manager.h"
#include "game/synchronization.h"
#ifdef __cplusplus
#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFileReader.h"
#endif

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */


struct SoundPlayerBase {
    const void *vtable;
    union SynchronizationStorage state;
    unsigned char flags[5];
    unsigned char reserved11[3];
    float volume;
    float pitch;
    float values1c[4];
    unsigned char priority;
    unsigned char reserved2d;
    unsigned char option;
    unsigned char reserved2f;
    float value30;
    unsigned char flags34[2];
    unsigned char reserved36[2];
    float values38[2];
    unsigned char reserved40[4];
};

struct SoundByteRamp {
    unsigned char start;
    unsigned char end;
    unsigned short duration;
    unsigned short elapsed;
};


struct SoundShortBufferDescriptor {
    void *data;
    unsigned length;
    unsigned char active;
    unsigned char reserved09[3];
};

struct SoundSmallDescriptor {
    const void *vtable;
    unsigned field04;
};

#ifdef __cplusplus
struct SoundStreamPlayer {
    struct SoundPlayerBase base;
    const void *control_vtable;
    unsigned control_fields[2];
    const void *parameter_vtable;
    unsigned parameter_fields[2];
    unsigned char reserved5c[0x1c];
    struct SoundByteRamp ramp;
    unsigned char reserved7e[0x12];
    void *buffers[16];
    unsigned char reservedd0[0x24];
    nw::snd::internal::WaveArchiveFileReader wave_archives[4];
    struct SoundShortBufferDescriptor short_descriptors[4];
    unsigned char reserved164;
    unsigned char flags165[2];
    unsigned char reserved167[0xd];
    unsigned fields174[4];
    unsigned field184;
    int field188;
    unsigned field18c;
    struct SoundSmallDescriptor small_descriptors[4];
};
#endif

struct SoundPoolStorage {
    void *begin;
    unsigned size;
};

#ifdef __cplusplus
struct EmbeddedSoundInstance {
    struct SoundPlaybackState base;
    unsigned char reserved118[4];
    struct SoundPoolStorage *owner;
    unsigned char flags120[2];
    unsigned char reserved122[6];
    struct SoundStreamPlayer player;
};
#endif

struct SoundSubPlayer {
    struct SoundPlayerBase base;
    const void *control_vtable;
    unsigned control_fields[2];
    const void *parameter_vtable;
    unsigned parameter_fields[2];
    unsigned char reserved5c[0x1c];
    struct TimingState timing;
    unsigned char reserved88[4];
    unsigned char flags8c[5];
    unsigned char reserved91[0xf];
    unsigned char flagsa0[4];
    struct SoundTaskRegistryEntry *registry;
    unsigned fieldsa8[7];
};

struct SoundRegularInstance {
    struct SoundPlaybackState base;
    unsigned char reserved118[4];
    struct SoundPoolStorage *owner;
    unsigned char flags120[2];
    unsigned char reserved122[2];
    struct SoundSubPlayer player;
};

struct SoundTaskBase {
    const void *vtable;
    struct PoolLink link;
    union SynchronizationStorage state;
    unsigned char active;
    unsigned char reserved15[3];
    void *owner;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct SoundPlaybackState *func_00180444(struct SoundPlaybackState *object);
extern struct SoundStreamPlayer *func_0014d690(struct SoundStreamPlayer *player);
extern const unsigned data_002246b8[];
extern struct SoundSubPlayer *func_0014d544(struct SoundSubPlayer *player);
extern const unsigned data_00224aac[];
extern void func_0019df28(struct StreamSemaphore *semaphore, int count);
extern struct EmbeddedSoundInstance *func_0014b824(struct EmbeddedSoundInstance *object,
                                             struct SoundPoolStorage *owner);
extern struct SoundRegularInstance *func_0014bd78(struct SoundRegularInstance *object,
                                              struct SoundPoolStorage *owner);
extern struct SoundTaskBase *func_00180510(struct SoundTaskBase *task);
#ifdef __cplusplus
}
#endif

#endif
