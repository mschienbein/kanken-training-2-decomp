#include "game/service_clients.h"

/* [INFERENCE] Checked-initialization operation spelling; no historical
 * source boundary or removed-callee identity is claimed. */
unsigned func_00192a7c(void) {
    unsigned result;
    if (data_00242c0c) result = 0;
    else result = 0xe0a046dbu;
    if (result != 0) {
        const unsigned address = __current_pc();
        func_0019d6c4(result, address);
    }
    return data_00242c08.handle;
}


