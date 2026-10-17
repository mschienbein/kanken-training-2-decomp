#include "game/sequence.h"
#include "game/audio_voice.h"



void func_001d4cb8(struct AnimationTrack *track)
{
    struct AudioVoice *voice;
    for (voice = track->activity.voices; voice; voice = voice->next)
        func_0017dae0(voice);
    track->activity.voices = 0;
}
