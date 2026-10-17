#include "game/audio_voice.h"
#include "game/sequence.h"
short *func_0016504c(struct SequencePlayer *player, int index)
{
    /* ORIGINAL BUG: negative indices are not rejected by the original signed bound check. */
    if (index < 16)
        return &player->variables[index];
    if (index < 32)
        return &bss_004b8984[index - 16];
    return 0;
}
