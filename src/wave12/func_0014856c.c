#include "game/object_manager.h"
#include "game/input_runtime.h"
#include "game/sound_controller.h"
#include "game/stream_manager.h"



void func_0014856c(struct InputSessionContext *object)
{
    if (!object->active) {
        func_00195ec4(&object->mutex);
        object->active = 1;
    }
}
