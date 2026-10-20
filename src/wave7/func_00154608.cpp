#include "game/audio_voice.h"

extern "C" void func_00154608(struct AudioChannel *element, unsigned int value)
{
    func_00154610(element->state, value);
}
