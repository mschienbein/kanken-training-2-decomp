#include "game/sequence.h"
#include "game/audio_voice.h"

/* The canonical suppress_stop field is the evidenced byte at offset 0x82;
 * the defining accepted voice-update endpoints share the header contracts. */

/* [INFERENCE] These scalar accessors and update helpers model a natural
 * source decomposition, not recovered historical helper identities. */
static int countdown_value(const int &remaining)
{
    return remaining;
}

static void set_countdown(int &remaining, int value)
{
    remaining = value;
}

static void decrement_countdown(int &remaining, int increment)
{
    if (countdown_value(remaining) > 0)
        set_countdown(remaining, countdown_value(remaining) - increment);
}

static void expire_voice(AnimationTrack *track, AudioVoice *voice)
{
    if (countdown_value(voice->value190) != 0 || voice->envelope.state == 4)
        return;
    if (!track->suppress_stop)
        func_001d9530(voice);
}

static void update_voice(AudioVoice *voice, AnimationTrack *track)
{
    decrement_countdown(voice->value190, 1);
    expire_voice(track, voice);
    if (!voice->flag13b)
        func_001d9178(voice, 1);
}

void func_001d4e2c(AnimationTrack *track)
{
    if (!track->active)
        return;
    AudioVoice *voice = track->activity.voices;
    if (!voice)
        return;
    do {
        update_voice(voice, track);
        voice = voice->next;
    } while (voice);
}
