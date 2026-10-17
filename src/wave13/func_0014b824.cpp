#include "game/sound_task.h"
#include <stddef.h>
#include "game/stream_manager.h"


#include "game/sound_runtime.h"



extern "C" EmbeddedSoundInstance *func_0014b824(EmbeddedSoundInstance *object,
                                             SoundPoolStorage *owner)
{
    object = (EmbeddedSoundInstance *)func_00180444(&object->base);
    object->base.vtable = data_002246b8;
    object->owner = owner;
    object->flags120[0] = false;
    object->flags120[1] = false;
    SoundStreamPlayer *player = func_0014d690(&object->player);
    return (EmbeddedSoundInstance *)((char *)player -
                                   offsetof(EmbeddedSoundInstance, player));
}
