#include "game/aux_effect.h"
#include "game/input_runtime.h"
int func_00151d50(struct AuxEffectState *effect, int enabled)
{
    if ((effect->channel == 0 || effect->channel == 1) && effect->buffer && effect->size) {
        struct ExtendedInputUpdate command;
        int channel;
        int result;
        command.value = enabled;
        command.flags = 1;
        channel = effect->channel;
        result = func_0014df74(func_001861e0(), channel, &command);
        if ((enabled & ~result) == 0)
            effect->enabled = enabled;
        if (!effect->enabled)
            effect->frame = bss_004b6584.frame;
        return result;
    }
    return 0;
}
