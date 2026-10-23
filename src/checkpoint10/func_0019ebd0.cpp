#include "game/resource_publication.h"

extern "C" void func_0019ebd0(unsigned int value)
{
    struct GameResourceState *state = &bss_00296758;
    CtrHandle *handle = &bss_00291758.completion;
    state->publish(value);
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
}
