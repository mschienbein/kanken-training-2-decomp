#ifndef GAME_VOICE_WAVE_INFO_H
#define GAME_VOICE_WAVE_INFO_H

#include "game/audio_filter.h"

struct AdpcmCoefficients {
    short pairs[8][2];
};
/* One real loop/history record is shared by waveform channels, stream
 * channel storage and RecordState::loop_context. */
struct AdpcmLoopState {
    short predictor_scale;
    short history1;
    short history2;
};
struct VoiceWaveChannel {
    const void *samples;
    struct AdpcmCoefficients coefficients;
    struct AdpcmLoopState initial, loop;
};
/* Target waveform descriptor, shared by the voice and channel initializers.
 * S10/S14: the stream starter intentionally initializes only format, count and
 * rate; Voice::Initialize reads only format and rate, not the unused tail. */
struct VoiceWaveInfo {
    unsigned char format, looping;
    unsigned char reserved02[2];
    int channel_count;
    int sample_rate;
    unsigned loop_start, loop_end, sample_count;
    struct VoiceWaveChannel channels[2];
};

#endif
