#include "ctr/svc.h"
#include "ctr/ipc.h"
#include "game/stream_manager.h"


#include "game/service_protocol.h"
struct ServiceReleaseState {
    unsigned char active;
    unsigned char reserved01[3];
    int references;
};
extern struct ServiceReleaseState data_00243774;

extern const unsigned data_00220b64;

static __forceinline void release_service(struct ServiceReleaseState *state)
{
    if (state->references > 0)
        --state->references;
    if (state->references == 0 && state->active) {
        int failure;
        state->active = 0;
        failure = (int)((unsigned)func_0012e7b4(&data_002438a8.handle) & 0x80000000u);
        if (failure >= 0)
            data_00243054 = data_00220b64;
    }
}

void func_00121520(void)
{
    struct StreamMutex *mutex = func_0018f734();
    func_00192bd8(mutex);
    release_service(&data_00243774);
    func_00195d74(mutex);
}
