#include "game/service_runtime.h"
#include "game/runtime.h"
/* [INFERENCE] This view refines reserved lifecycle bytes and the word at +8;
 * the canonical ResourceServiceState does not yet expose the session pointer. */
struct ResourceShutdownView {
    signed char status, active, callbacks_stopped, stopped;
    unsigned reserved04;
    const CtrHandle *session;
    unsigned busy;
    unsigned char lock_storage[8];
    CtrHandle handle;
};
struct ShutdownTimeSpan { long long nanoseconds; };
static struct ShutdownTimeSpan milliseconds(int value)
{
    struct ShutdownTimeSpan duration;
    duration.nanoseconds = (long long)value * 1000000;
    return duration;
}
extern int func_00186424(struct StreamCriticalSection *lock);
extern void func_001863b0(struct StreamCriticalSection *lock);
extern CtrResult func_001863fc(const CtrHandle *session);
CtrResult func_00121838(void)
{
    struct ResourceShutdownView *state = (struct ResourceShutdownView *)&data_00242afc;
    int i;
    CtrResult result;
    if (state->busy)
        func_0019f568();
    for (i = 0; i < 8; ++i)
        if (bss_0049eec4[i])
            func_0019f568();
    result = 0;
    if (state->active) {
        while (!func_00186424(&data_00242b0c))
            func_0011ef54(milliseconds(1).nanoseconds);
        result = func_001863fc(state->session);
        state->active = 0;
        func_001863b0(&data_00242b0c);
    }
    state->stopped = 1;
    return result;
}
