#include "game/audio_voice.h"
#include <string.h>
void func_0017da3c(struct RecordState *state)
{
    state->data = 0;
    state->sample_count = 0;
    state->loop_context = 0;
    state->sample_limit = 0;
    memset(&state->loop, 0, 4);
    state->next = 0;
    state->status = 0;
}
