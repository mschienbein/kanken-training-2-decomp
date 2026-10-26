#include "ctr/svc.h"
#include "game/runtime.h"
#include "game/stream_manager.h"

#include "game/service_lock.h"
#include "game/service_protocol.h"

struct TimeSpan { long long nanoseconds; };
static struct TimeSpan milliseconds(int value)
{
    struct TimeSpan result;
    result.nanoseconds = (long long)value * 1000000;
    return result;
}

void func_0018ef74(void)
{
    int i;
    if (data_00242afc.published & ~(signed char)data_00242afc.pending) {
        for (i = 0; i < 8; ++i)
            if (bss_0049eec4[i])
                bss_0049eec4[i]();
        if (data_00242afc.published) {
            while (!func_00186424(&data_00242b0c))
                func_0011ef54(milliseconds(1).nanoseconds);
            func_001863fc(data_00242afc.session);
            data_00242afc.published = 0;
            func_001863b0(&data_00242b0c);
        }
        data_00242afc.pending = 1;
    }
}
