#include "game/runtime.h"
#include "game/render_resource.h"

void func_00104abc(void)
{
    RendererShutdownState *state = &bss_0049d93c;
    if (state->allocation.buffer) {
        func_0019dc20(state->allocation.buffer);
        state->allocation.buffer = 0;
        state->allocation.count = 0;
        state->allocation.capacity = 0;
    }
    /* ORIGINAL BUG: shutdown is unconditional even if the second client is null. */
    state->clients[1]->shutdown();
    delete state->clients[1];
    delete state->clients[0];
    if (state->resource)
        func_001852f4(func_0010e1ac(state->resource));
}
