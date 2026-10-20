#include "game/audio_voice.h"

extern "C" void func_001d49e8(nw::snd::internal::driver::Voice *group, int index, const struct RecordState *slot)
{
    func_001c5994(group->channels[index], slot);
}
