#include "game/runtime.h"
#include "game/service_clients.h"
#include "game/result.h"

struct HardwareInfo {
    unsigned flags;
    unsigned reserved04[47];
};

/* INTEGRATOR: differs from src/wave4/func_00121764.c: callers read the IPC
 * reply from this buffer, so the buffer argument is writable, not const. */
/* INTEGRATOR: move to include/game/hardware_info.h */
extern "C" {
extern unsigned func_00121764(void *buffer, unsigned size, unsigned handle);

bool func_0010c06c(void);
}

static inline void check_result(Result result, unsigned location)
{
    switch (result.failed()) {
    case 0:
        break;
    default:
        func_0019d6c4(result.value, location);
        break;
    }
}


bool func_0010c06c(void)
{
    struct HardwareInfo info;
    Result result = func_00121764(&info, sizeof(info), 0xc0000);
    unsigned location = __current_pc();
    check_result(result, location);
    return (info.flags & 0x100u) != 0;
}
