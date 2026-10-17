#include "game/audio_voice.h"
#include "game/sequence.h"
short *func_00169f9c(struct AnimationTrack *track, int index)
{
    /* ORIGINAL BUG: negative indices are not rejected by the original signed bound check. */
    return index < 16 ? &track->variables[index] : 0;
}
