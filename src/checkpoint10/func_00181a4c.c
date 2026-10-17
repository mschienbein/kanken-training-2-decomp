#include "game/service_state.h"
/* [INFERENCE] Role names. The queue lives at bss_004990d0 (256 records of 32
 * bytes starting at +0x102c, write index at +0x1024); the sibling
 * func_00186598 reaches the same object through its tail at bss_0049a0d0 (+0x1000).
 * The fifth argument is one stacked 32-bit word that the original copies with
 * floating-point load/store instructions; callers pass integer bit patterns
 * (e.g. 4), so the one-word union below is [INFERENCE], not a recovered type. */

union QueueCommandArgument {
    float scalar;
    unsigned word;
};

struct QueuedCommand {
    unsigned type;
    unsigned state;
    unsigned reserved08;
    unsigned flags;
    union QueueCommandArgument size;
    unsigned target;
    const void *first;
    const void *last;
};

struct CommandQueue {
    unsigned char reserved00[0x1024];
    int next;
    unsigned reserved28;
    struct QueuedCommand commands[256];
};

/* INTEGRATOR: differs from include/game/service_state.h: WorkQueue ends at
 * +0x102c and types writeIndex as unsigned. This matching client accesses the
 * following 256 records and uses signed remainder for the index update.
 * Extend/unify the shared queue type before adopting this unit. */


/* INTEGRATOR: differs from include/game/command_state.h: that prototype is
 * (int, int, int, int, unsigned); the stacked fifth word is read as a float
 * object here, because only the float-member copy reproduces the original
 * vldr/vstr pair (a plain unsigned or whole-union copy uses core registers). */
void func_00181a4c(unsigned target, const void *first, const void *last,
                   unsigned flags, union QueueCommandArgument size)
{
    struct QueuedCommand *command = &bss_004990d0.commands[bss_004990d0.next];
    bss_004990d0.next = (bss_004990d0.next + 1) % 256;
    command->flags = flags;
    command->target = target;
    command->first = first;
    command->last = last;
    command->type = 3;
    command->state = 0;
    command->size.scalar = size.scalar;
}
