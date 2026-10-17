#include "game/service_callbacks.h"
#include "game/service_state.h"


extern void func_001923d4(unsigned mode);

/* [INFERENCE] Compiler-inlined mode operation shared with func_00115040. */
static __inline void set_mode(unsigned mode)
{
    switch (mode) {
    case 0:
        func_00192400(3);
        break;
    case 1:
        func_00192400(2);
        break;
    case 2:
        func_00192400(1);
        return;
    default:
        return;
    }
    func_001923d4(mode);
}

void func_0018ee88(unsigned char notify)
{
    data_00242a6c.status = 0;
    if (notify)
        set_mode(0);
}
