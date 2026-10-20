#include "game/screen.h"



static __inline void set_part_top(struct TextDisplayAttachment *part, int red,
                                int green, int blue,
                                int alpha)
{
    func_0015e57c(part->surface, red, green, blue, alpha);
}

void func_00170014(struct DisplayObject *object, int red,
                   int green, int blue, int alpha)
{
    set_part_top(display_text_attachment(object, 0), red, green, blue, alpha);
}
