#include "game/runtime.h"
#include "game/service_runtime.h"

#include "game/service_lock.h"
#include "game/service_protocol.h"

struct ShutdownTimeSpan { long long nanoseconds; };
static struct ShutdownTimeSpan shutdown_milliseconds(int milliseconds)
{
    struct ShutdownTimeSpan value;
    value.nanoseconds = (long long)milliseconds * 1000000;
    return value;
}

void func_001c5530(void *context)
{
    int index;
    int active = data_00242afc.published &
        ~(signed char)data_00242afc.pending;
    (void)context;
    if (active) {
        for (index = 0; index < 8; ++index)
            if (bss_0049eec4[index])
                bss_0049eec4[index]();
        if (data_00242afc.published) {
            while (!func_00186424(&data_00242b0c))
                func_0011ef54(shutdown_milliseconds(1).nanoseconds);
            func_001863fc(data_00242afc.session);
            data_00242afc.published = 0;
            func_001863b0(&data_00242b0c);
        }
        active = 1;
        data_00242afc.pending = 1;
    }
    data_00242afc.deferred = active;
}
