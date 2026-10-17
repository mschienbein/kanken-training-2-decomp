#include <stddef.h>
#include "game/adjusted_callbacks.h"

/* [INFERENCE] The virtual callback's return value is not observed. */

extern "C" void func_001d80a8(CallbackBase *callback)
{
    func_001d80b0(reinterpret_cast<CallbackHost68 *>(
        reinterpret_cast<unsigned char *>(callback) -
        offsetof(CallbackHost68, callback)));
}
