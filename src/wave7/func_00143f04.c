#include "game/archive.h"

struct SoundChannelState;

static __forceinline unsigned int align_word(unsigned int size)
{
    return (size + 3) & ~3u;
}

unsigned int func_00143f04(const struct SoundChannelState *state,
                          const struct SoundArchive *owner)
{
    return align_word(4 + 4 * func_0014a35c(owner));
}
