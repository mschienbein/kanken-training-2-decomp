#include "game/layout.h"
#include "game/audio_voice.h"

/* [INFERENCE] The returned update value models the two fields changed by
 * this setter; canonical member types and the public contract are unchanged. */
template <typename T>
static T maximum(T left, T right)
{
    if (left < right)
        left = right;
    return left;
}

struct ExtentUpdate {
    unsigned sample_rate;
    unsigned short flags;
};
static ExtentUpdate extent_update(int extent, unsigned short flags)
{
    ExtentUpdate update = { (unsigned)extent, (unsigned short)(flags | 2) };
    return update;
}


void func_001c5930(AudioChannelState *style, int extent)
{
    ExtentUpdate update = extent_update(maximum(extent, 0), style->flags);
    style->sample_rate = update.sample_rate;
    style->flags = update.flags;
}
