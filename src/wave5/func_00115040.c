#include "game/service_state.h"
extern void func_001923d4(unsigned mode);

void func_00115040(unsigned mode)
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
