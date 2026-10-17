#include "game/sequence.h"
#include "game/audio_voice.h"



void func_001d4db0(AudioVoice *voice, int event, void *context)
{
    AnimationTrack *track = static_cast<AnimationTrack *>(context);
    if (event == 0 || event == 2)
        func_0017dae0(voice);
    if (track->owner)
        track->owner->vtable->release_voice(track->owner, voice);
    AudioVoice *current = track->activity.voices;
    if (current == voice) {
        track->activity.voices = voice->next;
    } else {
        /* ORIGINAL BUG: the callback assumes the track's voice list is nonempty. */
        while (current->next) {
            if (current->next == voice) {
                current->next = voice->next;
                return;
            }
            current = current->next;
        }
    }
}
