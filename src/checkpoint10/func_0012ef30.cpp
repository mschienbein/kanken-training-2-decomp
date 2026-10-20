#include <string.h>
#include "game/applet.h"
#include "game/service_availability.h"
#include "game/result.h"
#include "game/exp_heap.h"


Result func_0012ef30(void)
{
    StreamLockGuard guard(&bss_004b4fb4);
    if (data_00243120 == 0) {
        func_0019edcc();
        Result result(func_00192b44(&data_00243794.handle, data_0021c61c,
                                    strlen(data_0021c61c), 0));
        switch (result.failed()) {
        case 0:
            break;
        default:
            return result;
        }
    }
    ++data_00243120;
    return Result();
}
