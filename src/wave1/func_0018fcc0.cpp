#include "game/resource.h"

extern "C" void func_001211b8(int mode);

extern "C" unsigned func_0018fcc0(unsigned *transferred, Resource *resource,
                                  unsigned long long offset, void *buffer,
                                  unsigned size)
{
    unsigned result;
    if (resource != 0 && buffer != 0) {
        func_001211b8(1);
        result = resource->read(transferred, offset, buffer, size);
    } else {
        result = 0xe0e046bcu;
    }
    return result;
}
