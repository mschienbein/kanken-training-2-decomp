#include "game/screen.h"


void func_001ad6d0(struct WidgetContainer *container, int first, int second)
{
    struct DisplayHandle *handle = container->handle;
    if (handle != 0 && handle->object != 0)
        func_00180358(&handle->object->base, first, second);
}
