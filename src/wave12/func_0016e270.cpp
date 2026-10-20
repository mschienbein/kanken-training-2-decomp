#define NW_RELEASE
#include "game/prompt.h"
#include "game/runtime.h"
#include "game/functions.h"
#include "game/screen.h"




extern "C" PromptManager *func_0016e270(void)
{
    if (!(data_0023eff8.initialized & 1) && func_00191c80(&data_0023eff8.word)) {
        PromptManager *manager = &data_0023effc;
        if (manager->handles[0] == 0) {
            PromptObjects *objects = static_cast<PromptObjects *>(
                func_0019ffa4(sizeof(PromptObjects), data_002232d5));
            if (objects != 0) {
                objects->objects[0] = 0;
                objects->objects[1] = 0;
            }
            manager->handles[0] = objects;
        }
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(manager, (ObjectDestructor)func_001c1970, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023eff8.word);
    }
    return &data_0023effc;
}
