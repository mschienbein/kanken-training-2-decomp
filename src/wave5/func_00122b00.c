#include "game/service_state.h"

int func_00122b00(void)
{
    unsigned state = func_00191de8();
    return func_0018eed0() && (state & 0x20000000u) && (state & 0x10000000u);
}
