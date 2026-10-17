#include "game/sound_task.h"
#include <stddef.h>
#include "game/stream_manager.h"


#include "game/sound_runtime.h"



extern "C" SoundRegularInstance *func_0014bd78(SoundRegularInstance *object,
                                              SoundPoolStorage *owner)
{
    object = (SoundRegularInstance *)func_00180444(&object->base);
    object->base.vtable = data_00224aac;
    object->owner = owner;
    object->flags120[0] = false;
    object->flags120[1] = false;
    SoundSubPlayer *player = func_0014d544(&object->player);
    return (SoundRegularInstance *)((char *)player -
                                    offsetof(SoundRegularInstance, player));
}
