#include <stddef.h>
#include "game/adjusted_callbacks.h"

/* [INFERENCE] The virtual callback's return value is not observed. */

extern "C" void func_001d5c98(CallbackBase *callback)
{
    func_001d5ca0(reinterpret_cast<CallbackHost80 *>(
        reinterpret_cast<unsigned char *>(callback) -
        offsetof(CallbackHost80, callback)));
}
