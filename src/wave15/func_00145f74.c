#include "game/functions.h"
#include "game/service_runtime.h"
#include "game/service_state.h"

typedef void (*ServiceCallback)(void);

static __inline void set_callback(ServiceCallback *callbacks, int index,
                                  ServiceCallback callback)
{
    callbacks[index] = callback;
}

int func_00145f74(ServiceCallback first, ServiceCallback second,
                  ServiceCallback third)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (bss_0049eec4[i] == 0) {
            bss_0049eec4[i] = first;
            set_callback(bss_0049eee4.resumed, i, second);
            set_callback(bss_0049eee4.exiting, i, third);
            return 1;
        }
    }
    return 0;
}
