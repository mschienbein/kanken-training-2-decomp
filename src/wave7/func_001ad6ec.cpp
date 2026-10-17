#include "game/screen.h"


void func_001ad6ec(struct WidgetContainer *container, WidgetPosition position)
{
    struct DisplayHandle *handle = container->handle;
    if (handle != 0 && handle->object != 0) {
        handle->x = position.x;
        handle->y = position.y;
    }
}
