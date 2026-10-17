#include "game/pool_list.h"
#include "game/service_state.h"
#include "game/sound_runtime.h"


static __forceinline void initialize_list(PoolList *list)
{
    list->count = 0;
    list->sentinel.next = &list->sentinel;
    list->sentinel.previous = &list->sentinel;
}

extern "C" SoundFileGroup *func_0014b60c(SoundFileGroup *object)
{
    initialize_list(&object->playing);
    initialize_list(&object->pending);
    initialize_list(&object->buffers);
    initialize_list(&object->available);
    object->buffer_count = 0;
    object->requested = 1;
    object->limit = 0x7fffffff;
    object->volume = 1.0f;
    object->fade = 0.0f;
    object->priority = -1;
    object->position[0] = 0.0f;
    object->position[1] = 0.0f;
    object->active = false;
    object->position[2] = 0.0f;
    object->velocity = 0.0f;
    return object;
}
