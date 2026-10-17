#include "game/command_buffer.h"



extern "C" unsigned char *func_001dacc0(void *object, unsigned size)
{
    data_00242fc8 = static_cast<W19CommandBuffer *>(object)->reserve(data_00242fc8, size);
    return data_00242fc8;
}
