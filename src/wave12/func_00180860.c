#include "game/audio_tasks.h"
#include "game/object_manager.h"
#include "game/sound_controller.h"
#include "game/pool_list.h"
#include "game/stream_manager.h"




void func_00180860(struct AudioTaskManager *manager)
{
    func_00195ec4(&manager->lock);
    func_00180884(&manager->queue, manager->storage, 0x20);
}
