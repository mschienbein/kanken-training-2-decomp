#include "game/functions.h"
/* [INFERENCE] Component type. func_001eb7a4 passes its +0x88
 * component. The reached count remains in r1 at the tail transfer;
 * func_001ae00c does not use that caller-evidenced input. */
#include "game/selection_components.h"

void func_001adff8(struct W19SelectionPanel *component)
{
    int index;
    for (index = 0; index < 25; ++index) {
    }
    func_001ae00c(component, index);
}
