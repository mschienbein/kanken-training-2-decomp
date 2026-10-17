#include "game/font.h"

void func_00113e14(struct FontResource *font, struct FontFile *file,
                   struct FontInformation *information)
{
    font->file = file;
    font->information = information;
}
