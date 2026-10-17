#include "game/font.h"
int func_001149c0(const struct FontFile *file, unsigned signature,
                  unsigned version, unsigned minimumBlocks)
{
    if (file->signature != signature || file->byteOrder != 0xfeff)
        return 0;
    if ((version >> 24) != (file->version >> 24))
        return 0;
    if (((version >> 16) & 255) < ((file->version >> 16) & 255))
        return 0;
    if ((version & 255) > (file->version & 255))
        return 0;
    if (file->fileSize >= 20 + minimumBlocks * 8 && file->blockCount >= minimumBlocks)
        return 1;
    return 0;
}
