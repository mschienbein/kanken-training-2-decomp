#include "game/audio_voice.h"


static __forceinline void set_ramp(struct AudioByteRamp *ramp,
                                  unsigned char target,
                                  unsigned short duration)
{
    unsigned char current;
    if (ramp->elapsed >= ramp->duration)
        current = ramp->end;
    else
        current = ramp->start +
            (ramp->end - ramp->start) * ramp->elapsed / ramp->duration;
    ramp->start = current;
    ramp->end = target;
    ramp->duration = duration;
    ramp->elapsed = 0;
}

extern "C" void func_001d4adc(struct AudioFadeController *controller, bool muted,
                   unsigned int milliseconds)
{
    struct AudioVoice *voice;

    controller->muted = muted;
    for (voice = controller->voices; voice != 0; voice = voice->next)
        set_ramp(&voice->ramp180, muted ? 0 : 255, (milliseconds + 4) / 5);
}
