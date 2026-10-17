#include "game/sound_track.h"
void func_001d3d30(struct StreamRange *range, void *buffer, unsigned size)
{
    range->buffer = buffer;
    range->size = size;
    range->position = -1;
    range->limit = -1;
}
