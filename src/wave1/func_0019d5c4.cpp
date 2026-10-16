#include "game/resource.h"

extern "C" void func_0019d5c4(Resource *resource)
{
    if (resource != 0)
        resource->release();
}
