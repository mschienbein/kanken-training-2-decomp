#include "game/service_protocol.h"

/* [INFERENCE] The register keyword preserves the delivered source/codegen
 * hint for the real output pointer; it is not evidence of original spelling. */
unsigned func_00138d7c(const struct ServiceResetState *object, register unsigned *entry_count)
{
    void *memory;
    unsigned result = func_0018fa44(func_0018fcb4(), &memory, object->generation);
    switch (result >> 31) {
    case 0: *entry_count = ((const struct ServiceList *)memory)->count; return 0;
    default: return result;
    }
}
