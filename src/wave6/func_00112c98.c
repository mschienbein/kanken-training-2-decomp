#include "game/runtime.h"
#include "game/service_protocol.h"
extern struct ServiceList bss_004b47d0;

/* [INFERENCE] 00112c80 forwards its handle word in r1; this reset ignores it. */
void func_00112c98(struct ServiceResetState *state, unsigned handle)
{
    struct ServiceList list;
    ++state->generation;
    /* ORIGINAL BUG: the reset copies an uninitialized local list into global state. */
    func_00192720(&bss_004b47d0, &list, sizeof(list));
}
