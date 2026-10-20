#include "game/audio_voice.h"

extern "C" void func_001c5994(AudioChannel *channel, const RecordState *slot)
{
    func_001c599c(channel->state, slot);
}
