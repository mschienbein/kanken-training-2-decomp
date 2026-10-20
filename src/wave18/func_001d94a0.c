#include "game/functions.h"
#include "game/audio_voice.h"


struct AudioSweep {
    unsigned char active;
    unsigned char reserved01[3];
    float value;
};
struct AudioModulationState {
    unsigned char reserved00[0x10];
    unsigned elapsed;
    float value;
    float scale;
    unsigned char active;
    unsigned char pending;
    unsigned char reserved1e[2];
};
struct VoiceAdpcmState { short predictor_scale, sample1, sample2; };
struct VoiceWaveChannel {
    const void *samples;
    struct AdpcmCoefficients coefficients;
    struct VoiceAdpcmState initial, loop;
};
struct VoiceWaveInfo {
    unsigned char format, looping;
    unsigned char reserved02[2];
    int channel_count;
    int sample_rate;
    unsigned loop_start, loop_end, sample_count;
    struct VoiceWaveChannel channels[2];
};
struct NoteVoice {
    unsigned char reserved00[0x98];
    struct AudioEnvelope envelope;
    struct AudioModulationState modulation[4];
    unsigned char reserved134[5];
    unsigned char active;
    unsigned char reserved13a[2];
    unsigned char option13c;
    unsigned char reserved13d[0x2f];
    unsigned reset16c;
    unsigned char reserved170[0x20];
    unsigned id;
    unsigned char reserved194[3];
    unsigned char mode197;
    unsigned char reserved198[0x10];
    struct AudioVoiceGroup *group;
};
extern const float data_00220c20;

extern void func_001d2d68(struct AudioSweep *sweep, float value);
extern void func_001d48d4(struct AudioVoiceGroup *group, const struct VoiceWaveInfo *info, unsigned sample_offset);
extern void func_001d9238(struct NoteVoice *voice, const struct VoiceWaveInfo *info, unsigned sample_offset);

void func_001d94a0(struct NoteVoice *voice, const struct VoiceWaveInfo *info,
                   unsigned id, unsigned sample_offset)
{
    int i;
    voice->id = id;
    for (i = 0; i < 4; ++i)
        func_001d98f4(&voice->modulation[i]);
    func_001d2d68((struct AudioSweep *)&voice->envelope, data_00220c20);
    voice->reset16c = 0;
    func_001d48d4(voice->group, info, sample_offset);
    func_001d9238(voice, info, sample_offset);
    func_001d4a48(voice->group, voice->mode197);
    func_001d4aac(voice->group);
    voice->active = 1;
}
