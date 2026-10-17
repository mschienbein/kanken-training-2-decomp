#include "game/object_manager.h"
#include "game/sound_controller.h"
#include "game/stream_manager.h"



void func_0014856c(struct LazyStreamMutex *object)
{
    if (!object->initialized) {
        func_00195ec4(&object->mutex);
        object->initialized = 1;
    }
}
