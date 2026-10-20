#include "game/screen.h"



static __inline void set_part_bottom(struct TextDisplayAttachment *part, int red,
                                   int green, int blue,
                                   int alpha)
{
    func_0015e534(part->surface, red, green, blue, alpha);
}

void func_0016fff8(struct DisplayObject *object, int red,
                   int green, int blue, int alpha)
{
    set_part_bottom(display_text_attachment(object, 0), red, green, blue, alpha);
}
