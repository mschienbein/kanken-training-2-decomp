#include "game/shared_memory.h"

extern struct SharedMemory *data_00242d88;

unsigned func_0010bfd8(void)
{
    return data_00242d88 ? data_00242d88->state : 3;
}
