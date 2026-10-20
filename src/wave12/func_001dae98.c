#include "game/font_resource.h"
void func_001dae98(struct FontResource *reader, struct FontFile *file,
                   struct FontInformation *information)
{
    reader->file = file;
    reader->information = information;
}
