#include "game/prompt.h"
#include "game/runtime.h"
#include "game/functions.h"
#include "game/screen.h"




extern "C" PromptManager *func_0016e270(void)
{
    if (!(data_0023eff8.initialized & 1) && func_00191c80(&data_0023eff8.word)) {
        PromptManager *manager = &data_0023effc;
        if (manager->objects == 0) {
            DisplayObject **objects = static_cast<DisplayObject **>(
                func_0019ffa4(2 * sizeof(DisplayObject *), data_002232d5));
            if (objects != 0) {
                objects[0] = 0;
                objects[1] = 0;
            }
            manager->objects = objects;
        }
        /* [INFERENCE] Typed source-only no-op registration; original call
         * identity is erased. It neither stores nor invokes this callback. */
        register_prompt_manager_cleanup(manager, func_001c1970, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023eff8.word);
    }
    return &data_0023effc;
}
