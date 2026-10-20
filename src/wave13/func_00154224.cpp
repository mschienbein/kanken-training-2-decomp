#include "game/audio_voice.h"
#include "game/service_runtime.h"



extern "C" void func_00154224(struct HandleManager *array)
{
    int i;
    for (i = 0; i < 24; ++i)
        func_0015510c(array->channels[i]->state);
}
