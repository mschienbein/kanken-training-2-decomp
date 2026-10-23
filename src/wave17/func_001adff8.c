#include "game/functions.h"
/* [INFERENCE] Component type. func_001eb7a4 passes its +0x88
 * component. The reached count remains in r1 at the tail transfer;
 * func_001ae00c does not use that caller-evidenced input. */
struct Component001adff8;

extern void func_001ae00c(struct Component001adff8 *component, int count);

void func_001adff8(struct Component001adff8 *component)
{
    int index;
    for (index = 0; index < 25; ++index) {
    }
    func_001ae00c(component, index);
}
