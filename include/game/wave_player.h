#ifndef GAME_WAVE_PLAYER_H
#define GAME_WAVE_PLAYER_H

struct W15iWavePlayer;
struct W15iWaveSound;
struct W15iSoundPool;
struct PendingTask68;

#include <stddef.h>
#include "game/audio_voice.h"
#include "game/sound_runtime.h"
#include "game/synchronization.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */

/* Shared pool/list prefix follows the natural pending-task cleanup. */
struct W15iSoundPool {
    unsigned char reserved00[0x14];
    PoolList playing;
    PoolList available;
};
struct W15iWavePlayer : SynchronizationOwner {
    unsigned char registered;
    unsigned char flag0d;
    unsigned char flag0e;
    unsigned char stopping;
    unsigned char reserved10[0x34];
    RegistryEntry entry;
    RegistryEntry managed_entry;
    unsigned char reserved5c[0x40];
    AudioVoice *voice;
    unsigned char reserveda0;
    unsigned char event_active;
    unsigned char managed;
    unsigned char reserveda3;
    W15iSoundPool *pool;
    PendingTask68 *item;
};
struct W15iWaveSound {
    SoundPlaybackState base;
    unsigned char reserved118[0x0c];
    W15iWavePlayer player;
};

struct W15iAddressCache {
    unsigned address;
    unsigned length;
    unsigned char active;
};
struct W15iRecordCache {
    unsigned words[3];
    unsigned char active;
};
struct W15iStreamVoice {
    unsigned char reserved00[0x1c];
    unsigned address;
};
struct W15iStreamItem;
struct W15iStreamSound;
struct W15iStreamVtable {
    unsigned char reserved00[0x0c];
    void (*stop)(W15iStreamSound *sound);
};
/* [INFERENCE] Registry-containing stream owner, distinct from decoder state.
 * Its entry is a genuine embedded subobject used by the invalidation callback. */
struct W15iStreamSound {
    const W15iStreamVtable *vtable;
    unsigned char reserved04[8];
    unsigned char registered;
    unsigned char flag0d;
    unsigned char flag0e;
    unsigned char stopping;
    unsigned char reserved10[0x34];
    RegistryEntry entry;
    unsigned char reserved50[0x40];
    W15iStreamVoice *voices[16];
    unsigned char reservedd0[0x24];
    W15iRecordCache records[4];
    W15iAddressCache caches[4];
    unsigned char reserved164;
    unsigned char event_active;
    unsigned char reserved166[0x0e];
    W15iSoundPool *pool;
    W15iStreamItem *item;
};

/* [INFERENCE] The three installed address points share this contiguous group. */
struct WavePlayerType {
    unsigned primary[12];
    unsigned registry[5];
    unsigned manager[7];
};

extern "C" {


extern const unsigned data_00224aac[];
extern const WavePlayerType data_002248e0;
extern const unsigned data_00224940[];
extern W15iWavePlayer *func_001d603c(W15iWavePlayer *player);
extern void func_001d5f1c(W15iWavePlayer *player);
/* [INFERENCE] Unlocated wave hook; the natural cleanup supplies this player. */
extern void func_001d59c0(W15iWavePlayer *player);
extern void func_001d3c58(W15iAddressCache *cache);
extern void func_001f4eb4(RegistryEntry *entry, unsigned begin, unsigned end);
}
#endif

#endif
