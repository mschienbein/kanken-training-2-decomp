#include "game/render_state.h"

typedef void *(*AllocationCallback)(unsigned module, unsigned kind,
                                    unsigned id, unsigned size);
typedef void (*ReleaseCallback)(unsigned module, unsigned kind,
                                unsigned id, void *memory);

void func_00215e88(AllocationCallback *allocate, ReleaseCallback *release)
{
    if (allocate != 0)
        *allocate = data_002429f4.allocate;
    if (release != 0)
        *release = data_002429f4.release;
}
