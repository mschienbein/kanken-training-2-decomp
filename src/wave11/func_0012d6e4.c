#include "game/service_state.h"

unsigned func_0012d6e4(int selection)
{
    switch (selection) {
    case 0:
        func_00136450("==", "string", "PricingSelection", "PURCHASABLE", "");
        break;
    case 1:
        func_00136450("==", "string", "PricingSelection", "RELEASED", "");
        break;
    default:
        return 0xe0e13884u;
    }
    return 0;
}
