#include "game/applet.h"


extern int func_00138274(void *buffer, unsigned size);

int func_00142fa4(void *buffer, unsigned size)
{
    int result;
    func_0019ecf8();
    result = func_00138274(buffer, size);
    func_0019ec88();
    return result;
}
