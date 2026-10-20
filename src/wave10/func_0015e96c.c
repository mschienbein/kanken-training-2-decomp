#include "game/screen.h"



void func_0015e96c(struct DisplayData *object, int index, unsigned char alpha)
{
    struct DisplayPart *part = object->parts[index];
    part->alpha = alpha;
    func_0015e3f4(part->surface, part->red, part->green, part->blue, alpha);
}
