#ifndef GAME_SOUND_WAVE_H
#define GAME_SOUND_WAVE_H

#include "game/archive.h"

#ifdef __cplusplus
#include <nw/snd/snd_WaveFile.h>

/* r334's admitted full-function witness uses the canonical image reader.
 * [INFERENCE] Playback-output names; the two channel records and all widths
 * follow the retained source. The unused origin is a nullable data pointer,
 * as in the public reader protocol; its callers supply null. */
struct SoundWaveChannel {
    const void *samples;
    nw::snd::DspAdpcmParam decoder;
    nw::snd::internal::DspAdpcmLoopParam loop_state;
};
struct SoundWavePlaybackInfo {
    unsigned char format;
    unsigned char loop;
    unsigned char reserved02[2];
    unsigned channel_count;
    unsigned sample_rate;
    unsigned loop_start;
    unsigned loop_end;
    unsigned original_loop_start;
    SoundWaveChannel channels[2];
};
extern "C" bool func_001f1cf0(const SoundWaveReader *reader,
                              SoundWavePlaybackInfo *out,
                              const void *wave_data_origin);
#endif

#endif
