#include "game/audio_voice.h"
#include <string.h>

void func_00150124(AudioVoiceGroup *group)
{
    AudioMixMatrix matrix;
    /* [INFERENCE] The emitted initialization performs both the float-valued
     * default construction and the caller's explicit all-zero reset. */
    memset(&matrix, 0, sizeof matrix);
    for (int i = 0; i < group->count; ++i) {
        AudioChannel *channel = group->channels[i];
        if (channel) {
            func_00152978(group, i, &matrix);
            func_001538f4(channel, &matrix);
        }
    }
}
