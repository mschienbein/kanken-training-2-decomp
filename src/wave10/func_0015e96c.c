#include "game/screen.h"


extern void func_0015e3f4(struct DisplaySurface *surface, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha);

void func_0015e96c(struct DisplayData *object, int index, unsigned char alpha)
{
    struct DisplayPart *part = object->parts[index];
    part->alpha = alpha;
    func_0015e3f4(part->surface, part->red, part->green, part->blue, alpha);
}
