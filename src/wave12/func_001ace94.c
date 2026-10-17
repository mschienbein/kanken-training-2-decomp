#include "game/audio.h"
#include "game/screen.h"



int func_001ace94(const struct Widget1ace94 *widget)
{
    const struct DisplayObject *object = widget->objects[0];
    if (object != 0)
        return func_00195874(object, 0) != 0;
    return 0;
}
