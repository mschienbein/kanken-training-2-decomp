#include "game/sequence.h"
#include "game/audio_voice.h"



void func_001d9530(struct AudioVoice *voice)
{
    if (!voice->reserved13d) {
        if (voice->envelope.state != 4) {
            if (voice->group && !voice->flag13c)
                func_00155148(voice->group, 1);
            voice->envelope.state = 4;
        }
        voice->flag138 = 0;
    }
}
