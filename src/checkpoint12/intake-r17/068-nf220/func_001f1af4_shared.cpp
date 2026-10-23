#include "game/archive.h"

/* [INFERENCE] Source-local ordinal helper for the serialized word table.
 * The unrelated floating-reader body is not adopted in this unit. */
namespace {
static unsigned count_present_options(const unsigned *flags, unsigned bit)
{
    bool present = false;
    int count = 0;
    for (unsigned index = 0; index <= bit; ++index) {
        if (*flags & (1u << index)) {
            ++count;
            if (index == bit)
                present = true;
        }
    }
    if (present)
        return count;
    return 0;
}

}


int func_0018279c(const unsigned *parameters, unsigned *value, unsigned bit)
{
    unsigned count = count_present_options(parameters, bit);
    if (!count)
        return false;
    *value = parameters[count];
    return true;
}
