#include "game/sound_track.h"
struct StorageArena *func_001da358(struct StorageArena *arena,
                                 unsigned int capacity)
{
    if (arena != 0)
        return func_001db07c(arena, capacity);
    return arena;
}
