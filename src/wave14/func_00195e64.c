#include "game/applet.h"
#include "game/service_callbacks.h"
#include <stdbool.h>


void func_00195e64(bool notify)
{
    data_00242a6c.status = 1;
    if (notify)
        func_0019242c();
}
