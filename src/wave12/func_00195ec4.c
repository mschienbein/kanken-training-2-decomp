#include "game/object_manager.h"
#include "game/sound_controller.h"
#include "game/stream_manager.h"


void func_00195ec4(struct StreamMutex *mutex)
{
    func_0012e560(&mutex->critical);
    mutex->owner = 0;
    mutex->depth = 0;
}
