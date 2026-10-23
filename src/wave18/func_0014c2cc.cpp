#include "game/sound_voice.h"

/* The native SoundStreamInstance constructor constructs four actual members
 * of this type. Preserve its compiler-generated array-construction protocol. */
SoundInterpolation::SoundInterpolation()
{
    value = 0.0f;
    target = 0.0f;
    remaining = 0;
    duration = 0;
}
