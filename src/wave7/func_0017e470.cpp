#include "game/audio_voice.h"

extern "C" void func_0017e470(nw::snd::internal::driver::Voice *group, unsigned int index,
                  const struct AdpcmCoefficients *source)
{
    func_001550e0(group->channels[index], source);
}
