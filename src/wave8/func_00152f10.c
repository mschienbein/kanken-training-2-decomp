#include "game/scene_lifecycle.h"


void func_00152f10(struct AudioVoiceGroup *resource, int event,
                   void *context)
{
    struct ResourceCallbackState *state = context;
    if (event == 0 || event == 1) {
        func_00183550(resource);
        state->completion = 0;
    } else if (event == 2 || event == 3) {
        state->completion = 0;
    }
}
