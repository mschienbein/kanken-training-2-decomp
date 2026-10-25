#include "game/service_clients.h"


extern "C" Result func_0011375c(unsigned char *selected)
{
    Result result = func_00121444();
    int failure = result.value & 0x80000000u;
    unsigned char value = 0;
    Result success(value);
    if (failure >= 0) {
        *selected = value;
        return success;
    }
    Result second = func_00121610();
    if ((int)(second.value & 0x80000000u) >= 0) {
        *selected = 1;
        return Result(0);
    }
    Result third = func_001216b8();
    if ((int)(third.value & 0x80000000u) >= 0) {
        *selected = 2;
        return Result(0);
    }
    return Result(0xd90103eau);
}
