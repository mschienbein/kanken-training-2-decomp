#include <new>
#include "game/service_list.h"
#include "game/service_state.h"


void func_00214138(void)
{
    /* [INFERENCE] Placement construction models the static-storage object's
     * lifetime initialization; original source spelling is unrecovered. */
    new (&data_002430e0) ServiceContext();
    func_001911c4(bss_004b45d0, func_001c4490,
                  sizeof(StaticServiceEntry), 64);
}
