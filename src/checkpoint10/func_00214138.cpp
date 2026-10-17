#include <new>
#include "game/service_list.h"
#include "game/service_state.h"

/* [INFERENCE] The original element class is unrecovered. Its size and count
 * are established by the initializer's array-construction call. No fields are
 * touched by the empty constructor; keep the element representation opaque. */
struct StaticServiceEntry {
    unsigned char reserved00[8];
};

/* INTEGRATOR: move to include/game/service_list.h. */
extern "C" {
extern StaticServiceEntry bss_004b45d0[64];
extern StaticServiceEntry *func_001c4490(StaticServiceEntry *entry);
/* Observed ARM veneer for the existing Thumb array-construction helper;
 * no external helper name or alternative target is invented. */
extern StaticServiceEntry *func_001911c4(
    StaticServiceEntry *entries,
    StaticServiceEntry *(*constructor)(StaticServiceEntry *),
    unsigned element_size, unsigned element_count);
void func_00214138(void);
}

void func_00214138(void)
{
    /* [INFERENCE] Placement construction models the static-storage object's
     * lifetime initialization; original source spelling is unrecovered. */
    new (&data_002430e0) ServiceContext();
    func_001911c4(bss_004b45d0, func_001c4490,
                  sizeof(StaticServiceEntry), 64);
}
