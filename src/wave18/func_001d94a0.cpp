#include "game/functions.h"
#include "game/audio_voice.h"


extern const float data_00220c20;


extern "C" void func_001d94a0(struct AudioVoice *voice, const struct VoiceWaveInfo *info,
                   unsigned id, unsigned sample_offset)
{
    int i;
    voice->value190 = id;
    for (i = 0; i < 4; ++i)
        func_001d98f4(&voice->modulation[i]);
    voice->envelope.Reset(data_00220c20);
    voice->value16c = 0;
    func_001d48d4(voice->group, info, sample_offset);
    func_001d9238(voice, info, sample_offset);
    func_001d4a48(voice->group, voice->mode197);
    func_001d4aac(voice->group);
    voice->flag139 = 1;
}
