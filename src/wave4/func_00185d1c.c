#include "game/functions.h"

extern volatile unsigned int data_00243738;
extern struct ObjectManager bss_004b8fa8;
extern struct ObjectManager *func_001807c0(struct ObjectManager *manager);

struct ObjectManager *func_00185d1c(void)
{
    if (!(data_00243738 & 1) && func_00191c80(&data_00243738)) {
        struct ObjectManager *manager = func_001807c0(&bss_004b8fa8);
        /* [INFERENCE] Typed source-only no-op registration; original call
         * identity is erased. It neither stores nor invokes this callback. */
        register_object_manager_cleanup(manager, func_00147260, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_00243738);
    }
    return &bss_004b8fa8;
}
