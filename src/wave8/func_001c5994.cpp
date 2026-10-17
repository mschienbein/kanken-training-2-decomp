#include "game/audio_voice.h"

extern "C" void func_001c5994(AudioChannel *channel, const AudioSlot *slot)
{
    func_001c599c(channel->state, slot);
}
