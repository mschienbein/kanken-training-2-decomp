#ifndef GAME_SEQUENCE_H
#define GAME_SEQUENCE_H

#include "game/audio_voice.h"
#ifdef __cplusplus
#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFileReader.h"
#endif

struct AnimationTrack;
struct AnimationTrackVTable;
struct AudioVoice;
struct BankReader;
struct Progress;
struct RegistryEntry;
struct SequencePlayer;
struct SequencePlayerVTable;
struct SequenceTrackAllocator;
struct SequenceTrackAllocatorVTable;
struct SequenceTrackState;
struct SequenceVariables;
struct TrackVoiceOwner;
struct TrackVoiceOwnerVTable;
struct VoiceCallbackState;
struct WordPair;

#include <stdbool.h>
#include "game/archive.h"
#include "game/functions.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct Progress {
    short value;
    short limit;
    short current;
};

struct SequenceTrackState {
    const unsigned char *begin;
    union { const unsigned char *end; int available; } input;
    unsigned char flag;
};

struct AnimationTrack {
    const struct AnimationTrackVTable *vtable;
    unsigned char reserved04, active;
    unsigned char reserved06[0x16];
    struct SequenceTrackState state;
    unsigned char reserved28[0x50];
    unsigned char dirty;
    unsigned char reserved79[3];
    int delay;
    unsigned char waiting;
    unsigned char reserved81;
    unsigned char suppress_stop;
    unsigned char reserved83[9];
    struct Progress progress[5];
    unsigned char reservedaa[0x62];
    short variables[16];
    struct TrackVoiceOwner *owner;
    /* [INFERENCE] The same word is tested as a count and walked as a voice list. */
    union { int wait_count; struct AudioVoice *voices; } activity;
};

struct SequencePlayerVTable {
    unsigned char reserved00[0x0c];
    void (*shutdown)(struct SequencePlayer *player);
};

struct SequenceTrackAllocatorVTable {
    unsigned char reserved00[0x0c];
    void (*release)(struct SequenceTrackAllocator *allocator, struct AnimationTrack *track);
};

struct SequenceTrackAllocator {
    const struct SequenceTrackAllocatorVTable *vtable;
};

#ifdef __cplusplus
struct SequencePlayer {
    const struct SequencePlayerVTable *vtable;
    unsigned char reserved04[8];
    unsigned char active, paused;
    unsigned char reserved0e[0x36];
    struct RegistryEntry player_entry, manager_entry;
    unsigned char reserved5c[0x28];
    struct SequenceTrackAllocator *allocator;
    void (*callback)(int event, struct SequenceVariables *variables, void *context);
    void *context;
    struct AnimationTrack *tracks[16];
    short variables[16];
    unsigned char reservedef4[4];
    nw::snd::internal::WaveArchiveFileReader wave_archives[4];
    struct BankReader banks[4];
    unsigned char state, reserved165, registered;
};
#endif

struct TrackVoiceOwnerVTable {
    unsigned char reserved00[0x20];
    void (*release_voice)(struct TrackVoiceOwner *owner, struct AudioVoice *voice);
};

struct TrackVoiceOwner {
    const struct TrackVoiceOwnerVTable *vtable;
};

struct VoiceCallbackState {
    unsigned char reserved00[0x9c];
    struct AudioVoice *voice;
};

struct SequenceVariables {
    short *player;
    short *global;
    short *track;
    unsigned char flag;
};

struct AnimationTrackVTable {
    unsigned char reserved00[8]; /* only the advance slot is evidenced here */
    int (*advance)(struct AnimationTrack *, int);
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
extern void func_00155148(nw::snd::internal::driver::Voice *group, int mode);
#endif
extern void func_0017db3c(struct AnimationTrack *track);
extern void func_0017daf8(struct AudioVoice *voice);
extern void func_0017dae0(struct AudioVoice *voice);
extern void func_0017e35c(struct SequencePlayer *player);
extern void func_0017deec(struct AnimationTrack *track);
extern void func_001d4db0(struct AudioVoice *voice, int event, void *context);
extern void func_00141594(const unsigned int *handle, unsigned int *out);
extern void func_00144960(void *const *value, void **out);
extern struct WordPair *func_0014d288(struct WordPair *pair);
extern void func_0014f7d8(const unsigned int *value, unsigned int *out);
extern bool func_00172f28(const unsigned int *kind);
extern void func_0017e2bc(struct AnimationTrack *track);
extern void func_0017e2d8(struct AnimationTrack *track, const unsigned char *begin, unsigned size);
extern void func_001d4cb8(struct AnimationTrack *track);
extern void func_001d5bf4(struct AudioVoice *voice, int event, void *context);
extern short bss_004b8984[];
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */




extern "C" {
struct AnimationTrack;
void func_0017db3c(AnimationTrack *track);
void func_0017daf8(AudioVoice *voice);
void func_001d4d5c(AnimationTrack *track, int release);
}
#endif

#endif
