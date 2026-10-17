#include "game/sequence.h"
#include "game/audio_voice.h"



void func_001d5bf4(struct AudioVoice *voice, int event, void *context)
{
    struct VoiceCallbackState *state = context;
    if (event == 2)
        func_0017dae0(voice);
    state->voice = 0;
}
