#include "game/service_state.h"

extern int func_0012f5a0(void);
extern int func_0012f608(void);
extern void func_00192afc(unsigned result, unsigned address);

static __inline void check_result(int result, unsigned address)
{
    switch ((unsigned)result >> 31) {
    case 0:
        break;
    default:
        func_00192afc((unsigned)result, address);
        break;
    }
}

void func_00191d34(unsigned char active)
{
    int result;
    if (active) {
        data_00242a28.graphics_active = 1;
        result = func_0012f5a0();
        check_result(result, __current_pc());
    } else if (data_00242a28.graphics_active) {
        data_00242a28.graphics_active = 0;
        result = func_0012f608();
        if ((unsigned)result != 0xd8a02a05u &&
            (unsigned)result != 0xd9001bf7u)
            check_result(result, __current_pc());
    }
}
