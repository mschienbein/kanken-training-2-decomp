#include "game/font_resource.h"
int func_001813a0(const struct BinaryFileHeader *file, unsigned signature,
                  unsigned version, unsigned minimum_blocks)
{
    if (file->signature != signature || file->byte_order != 0xfeff)
        return 0;
    if ((version >> 24) != (file->version >> 24))
        return 0;
    if (((version >> 16) & 0xff) < ((file->version >> 16) & 0xff))
        return 0;
    if ((version & 0xff) > (file->version & 0xff))
        return 0;
    if (20 + minimum_blocks * 8 <= file->file_size &&
        minimum_blocks <= file->block_count)
        return 1;
    return 0;
}
