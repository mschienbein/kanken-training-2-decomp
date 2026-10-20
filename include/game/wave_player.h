#ifndef GAME_WAVE_PLAYER_H
#define GAME_WAVE_PLAYER_H

struct W15iWavePlayer;
struct W15iWaveSound;
struct W15iSoundPool;
struct W15iWaveItem;

#include <stddef.h>
#include "game/audio_voice.h"
#include "game/sound_runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct W15iWavePlayer {
    const unsigned *vtable;
    unsigned char reserved04[8];
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
    W15iWaveItem *item;
};
struct W15iWaveSound {
    SoundPlaybackState base;
    unsigned char reserved118[0x0c];
    W15iWavePlayer player;
};

extern "C" {


extern const unsigned data_00224aac[];
extern W15iWavePlayer *func_001d603c(W15iWavePlayer *player);
}
#endif

#endif
