#include "game/service_protocol.h"
unsigned func_0012d7a0(struct ServiceObject *out, struct ServiceObject object)
{
    unsigned result;
    result = func_0019058c(&object);
    switch (result >> 31) {
    case 0:
        *out = object;
        return 0;
    default:
        return result;
    }
}
