#include <stddef.h>
#include "game/adjusted_callbacks.h"

/* [INFERENCE] The virtual callback's return value is not observed. */

extern "C" void func_001d7864(CallbackBase *callback)
{
    func_001d786c(reinterpret_cast<CallbackHost68 *>(
        reinterpret_cast<unsigned char *>(callback) -
        offsetof(CallbackHost68, callback)));
}
