#include "game/screen.h"



static __inline void set_part_top(struct DisplayPart *part, unsigned char red,
                                unsigned char green, unsigned char blue,
                                unsigned char alpha)
{
    func_0015e57c(part->surface, red, green, blue, alpha);
}

void func_00170014(struct DisplayData *object, unsigned char red,
                   unsigned char green, unsigned char blue, unsigned char alpha)
{
    set_part_top(object->parts[0], red, green, blue, alpha);
}
