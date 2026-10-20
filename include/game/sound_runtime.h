#ifndef GAME_SOUND_RUNTIME_H
#define GAME_SOUND_RUNTIME_H

#include <stdbool.h>
struct PoolLink;
struct PoolList;


#include "game/pool_list.h"
#include "game/functions.h"
#include "game/region_heap.h"

/* The allocator owns the first link word; enclosing pools pass this real
 * subobject rather than declaring the allocator as a pointer-to-pointer API. */
struct FreeListPool {
    struct FreeLink *head;
};

/* [INFERENCE] Only the pool prefix and its +8 allocator state are recovered. */
struct SoundPool {
    unsigned char reserved00[8];
    struct FreeListPool state;
};

struct SoundInstance;
struct SoundPlayer;
struct SoundHandle {
    struct SoundInstance *sound;
};

struct SoundInstanceVTable {
    unsigned char reserved00[0x10];
    void (*stop)(struct SoundInstance *sound);
    unsigned char reserved14[0x10];
    void (*priority_changed)(struct SoundInstance *sound);
};

struct SoundInstance {
    const struct SoundInstanceVTable *vtable;
    unsigned char reserved04[4];
    struct SoundHandle *handle;
    struct SoundHandle *secondary_handle;
    struct SoundPlayer *player;
    unsigned char reserved14[4];
    unsigned selection_state;
    unsigned char reserved1c[0x34];
    int priority_offset;
    unsigned char reserved54[0x49];
    unsigned char priority;
    unsigned char reserved9e[0x2a];
    float volume;
    unsigned char reservedcc[0x2c];
    struct PoolLink pool_link;
    struct PoolLink play_link;
    struct PoolLink player_link;
    struct PoolLink selection_link;
};
#ifdef __cplusplus
#include <preinclude.h>
#include <nw/ut/ut_LinkList.h>
/* The same actual sound receiver has play/priority link subobjects at
 * +0x100/+0x108. This is a target-version prefix, not the public class size. */
typedef nw::ut::LinkList<SoundInstance, offsetof(SoundInstance, play_link)> PlaySoundList;
typedef nw::ut::LinkList<SoundInstance, offsetof(SoundInstance, player_link)> PrioritySoundList;
struct SoundPlayer {
    PlaySoundList playList;
    PrioritySoundList priorityList;
};
#endif

/* [INFERENCE] Observed control prefixes, not complete original class sizes. */
struct SoundInstancePool {
    unsigned char reserved00[8];
    struct PoolList active;
    struct PoolList available;
};
struct SequenceSoundControl {
    struct SoundInstance sound;
    void *source;
    struct SoundInstancePool *pool;
    unsigned char active;
    unsigned char enabled;
};
struct WaveSoundControl {
    struct SoundInstance sound;
    void *source;
    struct SoundInstancePool *pool;
    unsigned char active;
    unsigned char enabled;
};

/* [INFERENCE] The member address at +0x2314 is used as a 32-bit task ID.
 * Its original class is unrecovered; opaque storage does not assert a Task
 * or RegistryEntry owner. Pointer-to-ID conversion is the pinned ARM ABI. */
struct SequencePoolSound {
    struct SoundInstance sound;
    unsigned char reserved118[0x2314 - 0x118];
    unsigned char task_identity[0x3044 - 0x2314];
    struct SoundInstancePool *pool;
    unsigned char reserved3048[0x328e - 0x3048];
    bool active;
#ifdef __cplusplus
    bool isActive() const { return active; }
    void setInactive() { active = 0; }
#endif
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
#ifdef __cplusplus
    SoundPlaybackState();
#endif
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
/* [INFERENCE] Options copied by the two sequence-start command paths. */
struct SequenceStartOptions {
    unsigned char paused;
    unsigned char reserved01[3];
    unsigned position;
    unsigned offset;
    unsigned limit;
};

#ifdef __cplusplus
/* [INFERENCE] Command-owner/binding views retain the natural preparation
 * receiver and start-options contract; no historical complete class is named. */
struct SequenceDataBinding {
    unsigned reserved00[5];
    const void *data;
};
struct SequenceCommandSound : SequenceSound {
    unsigned char reserved230[0x20e4];
    SequenceDataBinding binding;
};
struct SequenceStartEvent : StreamEvent {
    unsigned reserved14;
    SequenceStartOptions options;
    SequenceDataBinding *binding;
    StreamEvent *initialize(SoundPlaybackState *player, SequenceDataBinding *new_binding,
                            const SequenceStartOptions &new_options)
    {
        opcode = 0x28;
        object = player;
        binding = new_binding;
        options = new_options;
        return this;
    }
};
extern "C" void func_001d31cc(SequenceCommandSound *sound, const void *data,
                             const SequenceStartOptions *options);
#endif

struct SoundParameterTarget {
    unsigned char reserved00[0x83];
    unsigned char flag83;
    unsigned char reserved84[0x6c];
    unsigned char flagf0;
    unsigned char reservedf1[3];
    unsigned char flagf4;
};

struct SoundPriorityCallback;
struct SoundPriorityCallbackVTable {
    unsigned char reserved00[0x0c];
    int (*priority_offset)(struct SoundPriorityCallback *, void *, unsigned sound_id);
};
struct SoundPriorityCallback { const struct SoundPriorityCallbackVTable *vtable; };
struct SoundPriorityHook {
    struct SoundPriorityCallback *callback;
    unsigned char reserved04[8];
    void *context;
#ifdef __cplusplus
    int priorityOffset(unsigned sound_id) const
    {
        return callback ? callback->vtable->priority_offset(callback, context, sound_id) : 0;
    }
#endif
};
struct SoundArchivePlayer;
struct SoundStartInfo;
struct SoundStartActor;
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned func_00180768(struct FreeListPool *pool, void *storage,
                             unsigned size, unsigned element_size);
extern unsigned func_0018075c(struct SoundPool *pool, void *storage, unsigned size);
/* Signed native priorities remain full words at the API; only the stored
 * priority member is a byte (BasicSound::SetPriority(int,int)). */
extern void func_0016a87c(struct SoundInstance *sound, int priority, int offset);
extern void func_001d2c84(struct SoundInstance *sound, int priority);
extern void func_00143328(struct SoundPlayer *player, struct SoundInstance *sound);
/* [INFERENCE] Native DetachSoundPlayer takes the player argument even though
 * this body only clears the stored pointer; caller inputs are not discarded. */
extern void func_001d2c78(struct SoundInstance *sound, struct SoundPlayer *player);
extern void func_001d2c44(struct SoundPlayer *player, struct SoundInstance *sound);
extern void func_0016a3e0(struct SoundInstance *sound);
extern void func_001d3bcc(struct SequenceSoundControl *sound);
extern void func_001d9c14(struct WaveSoundControl *sound);
extern void func_001d3230(struct SequencePoolSound *sound);
extern void func_001d2f90(struct SequenceSound *sound, const void *data,
    const struct SequenceStartOptions *options);
extern void func_0016a560(struct SoundInstance *sound);
extern void func_001d38ac(struct SequenceSoundControl *sound);
extern void func_001d9a0c(struct WaveSoundControl *sound);
extern int func_001d2cb4(const struct SoundPriorityHook *hook, unsigned sound_id);
extern const unsigned data_0022459c[];
extern const unsigned data_002245d8[];
extern void func_00190634(struct SoundInstance *sound);
extern void func_0019063c(struct SoundHandle *handle);
extern int func_001d2ce0(const struct SoundInstance *sound);
extern struct SoundInstance *func_001d4464(const struct SoundSelectionList *selection);
extern void func_001d2d14(struct SoundInstance *object, struct PoolList *registry);
extern unsigned func_001d0614(struct SoundArchivePlayer *player,
    struct SoundHandle *handle, unsigned sound_id, struct SoundInstance *attached,
    struct SoundStartActor *actor, int mode, const struct SoundStartInfo *info);
extern unsigned char func_001d0380(struct SoundArchivePlayer *player,
    struct SoundHandle *handle, unsigned sound_id, int mode,
    const struct SoundStartInfo *info);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned data_002245ec[];
#ifdef __cplusplus
}
#endif

#endif
