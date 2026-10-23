#include "game/resource.h"

Result func_001c4a8c(int *transferred, Resource *resource,
    long long offset, const void *buffer, unsigned size, int options)
{
    Result result;
    if (transferred != 0 && resource != 0 && buffer != 0) {
        func_001211b8(0);
        result = resource->write(transferred, offset, buffer, size, options);
    } else {
        result = Result(0xe0e046bcu);
    }
    return result;
}
