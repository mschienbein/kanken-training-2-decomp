#include "game/functions.h"
#include "game/screen.h"

#include "game/display_update.h"

void func_001eb7a4(struct DisplayUpdateManager *manager)
{
    int index;
    if (manager->component38 != 0) func_001dcf4c(manager->component38);
    if (manager->component3c != 0) func_001dcdd0(manager->component3c);
    if (manager->component40 != 0) func_001e358c(manager->component40);
    if (manager->component44 != 0) func_001b3564(manager->component44);
    if (manager->component48 != 0) func_001eaaac(manager->component48);
    if (manager->component4c != 0) func_001e9e7c(manager->component4c);
    if (manager->component50 != 0) func_001eaf70(manager->component50);
    if (manager->component54 != 0) func_0015eb90(manager->component54);
    /* [INFERENCE] callee identity: inlined by armlink. Only an empty
     * object operation is recoverable for this component. */
    if (manager->component58 != 0) func_001acc18(manager->component58);
    if (manager->component5c != 0) func_001ad134(manager->component5c);
    if (manager->component60 != 0) func_001acc1c(manager->component60);
    if (manager->component64 != 0) func_001b6b88(manager->component64);
    if (manager->component68 != 0) func_001b4718(manager->component68);
    if (manager->component6c != 0) func_001b4310(manager->component6c);
    if (manager->component70 != 0) func_001af244(manager->component70);
    if (manager->component74 != 0) func_001acebc(manager->component74);
    if (manager->component78 != 0) func_001ad7a0(manager->component78);
    if (manager->component7c != 0) func_001b6098(manager->component7c);
    if (manager->component80 != 0) func_001b4d58(manager->component80);
    if (manager->component84 != 0) func_001b7c88(manager->component84);
    if (manager->component88 != 0) func_001adff8(manager->component88);
    if (manager->component8c != 0) func_001b6604(manager->component8c);
    if (manager->count4c > 0) {
        index = 0;
        do {
            func_001ebb14(manager, &manager->entries4c[index]);
            ++index;
        } while (index < manager->count4c);
    }
    if (manager->count50 > 0) {
        index = 0;
        do {
            func_001eb95c(manager, &manager->entries50[index]);
            ++index;
        } while (index < manager->count50);
    }
    if (manager->count6c > 0) {
        index = 0;
        do {
            func_001ebca0(manager, &manager->entries6c[index]);
            ++index;
        } while (index < manager->count6c);
    }
}
