#include "game/audio_voice.h"
/* [INFERENCE] Kind, option and flags roles are opaque; callers evidence six inputs. */
void func_001719f4(struct ConfigState001719f4 *state, unsigned char kind, int format,
                  unsigned char option, int index, unsigned flags)
{
    state->kind = kind;
    state->complex_format = func_001dc390(format);
    state->index = index;
    state->option = option;
    func_00171a34(state->target, option, index, flags);
}
