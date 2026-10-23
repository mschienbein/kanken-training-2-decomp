#ifndef GAME_SOUND_TASK_H
#define GAME_SOUND_TASK_H

struct EmbeddedSoundInstance;
struct PoolLink;
struct SignalState;
struct SoundBufferDescriptor;
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

struct SoundBufferDescriptor {
    void *data;
    unsigned length;
    unsigned position;
    unsigned char active;
    unsigned char reserved0d[3];
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

/* Original construction and destruction address the two interface tables
 * within one primary type-table group, at +0x34 and +0x48. Uncalled entries
 * remain opaque; this declares layout, not invented callable signatures. */
struct SoundStreamPlayerTypeTables {
    unsigned primary[13];
    unsigned control[5];
    unsigned parameter[5];
};

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
    struct SoundBufferDescriptor descriptors[4];
    struct SoundShortBufferDescriptor short_descriptors[4];
    unsigned char reserved164;
    unsigned char flags165[2];
    unsigned char reserved167[0xd];
    unsigned fields174[4];
    unsigned field184;
    int field188;
    unsigned field18c;
    struct SoundSmallDescriptor small_descriptors[4];
#ifdef __cplusplus
    ~SoundStreamPlayer();
#endif
};

struct SoundPoolStorage {
    void *begin;
    unsigned size;
};

struct EmbeddedSoundInstance {
    struct SoundPlaybackState base;
    unsigned char reserved118[4];
    struct SoundPoolStorage *owner;
    unsigned char flags120[2];
    unsigned char reserved122[6];
    struct SoundStreamPlayer player;
#ifdef __cplusplus
    ~EmbeddedSoundInstance();
#endif
};

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
struct SoundQueuedTask {
    struct SoundTaskBase base;
    unsigned arguments[11];
    unsigned char option;
    unsigned char reserved49[3];
    void *source;
    struct PoolLink queue_link;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct SoundPlaybackState *func_00180444(struct SoundPlaybackState *object);
extern struct SoundStreamPlayer *func_0014d690(struct SoundStreamPlayer *player);
extern const unsigned data_002246b8[];
extern struct SoundSubPlayer *func_0014d544(struct SoundSubPlayer *player);
/* [INFERENCE] Original construction at 0014b824 and the shared type table
 * identify the +0x128 player; these names describe that target owner only. */
extern void func_001d8d7c(struct SoundStreamPlayer *player);
extern const struct SoundStreamPlayerTypeTables data_00224a00;
extern const unsigned data_00224940[];
extern const unsigned data_00224aac[];
extern void func_0019df28(struct StreamSemaphore *semaphore, int count);
extern struct EmbeddedSoundInstance *func_0014b824(struct EmbeddedSoundInstance *object,
                                             struct SoundPoolStorage *owner);
extern struct SoundRegularInstance *func_0014bd78(struct SoundRegularInstance *object,
                                              struct SoundPoolStorage *owner);
extern struct SoundTaskBase *func_00180510(struct SoundTaskBase *task);
extern const unsigned data_00224978[];
extern struct SoundQueuedTask *func_0014d664(struct SoundQueuedTask *task);
#ifdef __cplusplus
}
#endif

#endif
