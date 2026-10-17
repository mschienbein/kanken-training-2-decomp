#include "game/applet.h"
#include "game/exp_heap.h"
#include "game/service_availability.h"
#include "game/service_result.h"
#include "game/service_state.h"
#include <string.h>
#include "game/stream_manager.h"
#include "game/result.h"



static __forceinline int has_references(void)
{
    StreamLockGuard guard(&bss_0049ee70);
    return data_00242a1c.references != 0;
}

Result func_0010b628(void)
{
    StreamLockGuard guard(&bss_0049ee70);
    if (func_0012d43c())
        return Result(0xd8a09ff9u);
    if (has_references()) {
        ++data_00242a1c.references;
        return Result(0x209ff9);
    }
    Result code(func_0019edcc());
    switch (code.failed()) {
    case 0:
        break;
    default:
        return code;
    }
    code = Result(func_00192b44(&data_00242ff8, data_0021ab9c,
                               strlen(data_0021ab9c), 0));
    switch (code.failed()) {
    case 0:
        break;
    default:
        return code;
    }
    ++data_00242a1c.references;
    func_00112518(0xb0502c8);
    return Result(0);
}
