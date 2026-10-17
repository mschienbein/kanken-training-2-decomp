#include "game/sound_runtime.h"
#include "game/pool_list.h"



/* The caller-evidenced registry argument is ignored by the original setter.
 * [INFERENCE] The cleared word represents state; its narrower role is unknown. */
void func_001d2d14(struct SoundInstance *object, struct PoolList *registry)
{
    object->selection_state = 0;
}
