#include "game/screen.h"


void func_0015e684(struct DisplayObject *object, int index,
                   float x, float y, float z)
{
    struct TextDisplayAttachment *binding = display_text_attachment(object, index);
    object->position[0] = x;
    object->position[1] = y;
    object->position[2] = z;
    func_0015e698(binding->surface, x, y, z);
}
