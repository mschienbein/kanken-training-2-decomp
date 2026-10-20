#include "game/sequence.h"
#include "game/audio_voice.h"



void func_001d4d5c(AnimationTrack *track, int release)
{
    func_0017db3c(track);
    for (AudioVoice *voice = track->activity.voices; voice; voice = voice->next) {
        if (voice->flag139) {
            if (release >= 0)
                func_0016a204(&voice->envelope, (unsigned char)release);
            func_0017daf8(voice);
        }
    }
}
