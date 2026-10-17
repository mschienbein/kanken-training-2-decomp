#include "game/sequence.h"
#include "game/audio_voice.h"

void func_0017dae0(struct AudioVoice *voice)
{
    if (voice) {
        voice->callback = 0;
        voice->callback_data = 0;
    }
}
