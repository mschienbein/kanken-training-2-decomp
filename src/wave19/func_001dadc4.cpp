#include "game/command_buffer.h"



extern "C" unsigned char *func_001dadc4(void *object, unsigned size)
{
    data_00242fc8 = static_cast<W19CommandBuffer *>(object)->commit(data_00242fc8, size);
    return data_00242fc8;
}
