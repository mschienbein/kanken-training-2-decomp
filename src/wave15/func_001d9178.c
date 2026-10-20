#include "game/audio_voice.h"

void func_001d9178(struct AudioVoice *voice, int increment)
{
    voice->value16c += increment;
    if (voice->value16c > voice->value170)
        voice->value16c = voice->value170;
}
