#include "game/audio_voice.h"
#include "game/display_animation.h"
#include "game/service_runtime.h"
/* The calling setter supplies its volume-state owner before the value. */
int func_001462d4(struct AudioVolumeState *owner, unsigned short value)
{
    return func_00148a3c(&bss_004b5284, value);
}
