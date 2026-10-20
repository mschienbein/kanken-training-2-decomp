#include "game/screen.h"



void func_0015e96c(struct DisplayObject *object, int index, unsigned char alpha)
{
    struct TextDisplayAttachment *part = display_text_attachment(object, index);
    part->timed.common.alpha = alpha;
    func_0015e3f4(part->surface, part->timed.common.red, part->timed.common.green, part->timed.common.blue, alpha);
}
