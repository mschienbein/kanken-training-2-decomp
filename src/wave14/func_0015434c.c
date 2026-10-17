#include "game/audio_voice.h"
#include "game/display_animation.h"
#include "game/service_runtime.h"
void func_0015434c(struct ServiceOutput *renderer)
{
    int count = func_001549d8(&bss_004b5284);
    if (count >= 0)
        renderer->consumed_samples += count;
}
