#include "game/service_state.h"

/* func_00122b30 supplies the stored context word registered through func_0010ce58.
 * [INFERENCE] Opaque pointer type: the observed registration supplies null. */
int func_0010b610(void *context)
{
    return func_00192c10() == 0 ? 1 : 2;
}
