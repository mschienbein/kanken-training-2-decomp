#include "game/font_resource.h"
void func_001dae98(struct FontReader *reader, struct BinaryFileHeader *file,
                   struct FontInfo *info)
{
    reader->file = file;
    reader->info = info;
}
