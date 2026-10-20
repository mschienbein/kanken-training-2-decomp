#include "game/startup.h"
#include "game/exp_heap.h"

unsigned data_0023eeb4 = 0;
unsigned data_0023eeb8 = 0;
unsigned data_0023eebc = 0;

static __inline void reset_startup_entries(struct StartupCommandPool *pool)
{
    int index = 0;
    struct StartupCommandEntry *entry = pool->entries;
    for (;;) {
        entry->command = 0;
        entry->argument = 0;
        ++entry;
        if (++index >= 0x300)
            break;
    }
}

/* [INFERENCE] Natural flag-initialization phase; historical helper identity
 * and translation-unit boundary are not established. This is compiler-inlined,
 * not an address-bound or removed armlink callee. */
static void initialize_flags(unsigned *ready, unsigned *enabled, unsigned *active)
{
    *ready = 0;
    *enabled = 0;
    *active = 0;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_001415b0();
    if (*ready == 0)
        *ready = 1;
    if (*enabled == 0)
        *enabled = 1;
    if (*active == 0)
        *active = 1;
}

void func_00141944(void)
{
    bss_0028d944.commands = func_0019d798(1, 4, sizeof(struct StartupCommandPool));
    func_00135268(1, 0x300);
    func_00135128(0x100);
    func_00135f1c(8);
    func_00135c74(0x200, 8, 8);
    func_00135c10(0x400);
    func_00135bb0(0x400);
    func_00135b4c(0x400);
    func_001360dc(1);
    func_0013604c(4);
    func_0013605c(10);
    func_0013606c(1);
    func_0013607c(1);
    func_00145afc(0);
    func_0019f450(60.0f);
    func_00190678();
    bss_0028d944.commands->storage = func_0019d798(3, 0x80, 0x80000);

    reset_startup_entries(bss_0028d944.commands);
    bss_0028d944.commands->count = 0;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_001415b0();
    func_00145690();
    func_00145658(1);
    initialize_flags(&data_0023eeb8, &data_0023eebc, &data_0023eeb4);
    bss_0028d944.status = 100;
    bss_0028d944.counter = 0;
    bss_0028d944.pending = 0;
    bss_0028d944.ready = 1;
}
