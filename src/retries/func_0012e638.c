#include "game/service_state.h"
#include "ctr/cp15.h"


static int thread_slot_is_allocated(unsigned int slot)
{
    return slot < 16 && ((data_00243124 >> slot) & 1u) != 0;
}

void func_0012e638(void)
{
    unsigned int slot = 0;
    do {
        void (*destructor)(void *) = bss_004b51e8[slot];
        if (thread_slot_is_allocated(slot) && destructor)
            destructor(((void **)ctrGetThreadLocalStorage())[slot]);
        ++slot;
    } while ((int)slot < 16);
}
