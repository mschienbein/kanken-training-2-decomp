#include <string.h>
#include "game/applet.h"
#include "game/service_availability.h"
#include "game/result.h"
#include "game/exp_heap.h"

/* INTEGRATOR: move to include/game/service_availability.h */
extern "C" {
extern struct StreamMutex bss_004b4fb4;
extern int data_00243120;
extern unsigned data_00243794;
extern const char data_0021c61c[];
Result func_0012ef30(void);
}

Result func_0012ef30(void)
{
    StreamLockGuard guard(&bss_004b4fb4);
    if (data_00243120 == 0) {
        func_0019edcc();
        Result result(func_00192b44(&data_00243794, data_0021c61c,
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
