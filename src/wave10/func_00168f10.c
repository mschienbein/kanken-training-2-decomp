#include "game/screen.h"


void func_00168f10(struct WidgetController *screen, int enabled)
{
    int index;
    for (index = 19; index <= 26; ++index)
        func_0016e7e0(screen, index, enabled);
}
