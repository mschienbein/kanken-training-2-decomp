#include "game/sequence.h"
#include "game/audio_voice.h"



void func_0017deec(struct AnimationTrack *track)
{
    struct AudioVoice *voice;
    func_0017db3c(track);
    for (voice = track->activity.voices; voice; voice = voice->next) {
        if (voice->flag139)
            func_0017daf8(voice);
    }
    for (voice = track->activity.voices; voice; voice = voice->next)
        func_0017dae0(voice);
    track->activity.voices = 0;
    track->active = 0;
}
