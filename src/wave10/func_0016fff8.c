#include "game/screen.h"


extern void func_0015e534(struct DisplaySurface *surface, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha);

static __inline void set_part_bottom(struct DisplayPart *part, unsigned char red,
                                   unsigned char green, unsigned char blue,
                                   unsigned char alpha)
{
    func_0015e534(part->surface, red, green, blue, alpha);
}

void func_0016fff8(struct DisplayData *object, unsigned char red,
                   unsigned char green, unsigned char blue, unsigned char alpha)
{
    set_part_bottom(object->parts[0], red, green, blue, alpha);
}
