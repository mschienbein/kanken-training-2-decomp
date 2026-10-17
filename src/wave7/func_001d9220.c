#include "game/audio_voice.h"
void func_001d9220(struct AudioVoice *voice, float value, int duration, unsigned char enabled)
{
    voice->value168 = value;
    voice->value16c = 0;
    voice->value170 = duration;
    voice->flag13b = enabled;
}
