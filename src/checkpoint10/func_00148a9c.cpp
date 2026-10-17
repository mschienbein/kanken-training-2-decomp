#include "ctr/svc.h"
#include "game/synchronization.h"
struct CoordinatorRequest {
    unsigned int flags;
    unsigned char reserved04[26];
    unsigned short value;
};
struct ServiceBufferPair { unsigned int first, second; };
/* INTEGRATOR: differs from include/game/service_runtime.h: completion_slots
 * point to request records with flags at +0 and a value at +0x1e. */
struct ServiceCoordinator {
    CtrHandle ready_event;
    CtrHandle completion_event;
    struct StreamMutex mutex;
    unsigned char reserved14[0x109c - 0x14];
    struct CoordinatorRequest *completion_slots[150];
    struct ServiceBufferPair buffers[5];
    unsigned char enabled, pending;
    unsigned short serial, current_buffer, completion_slot;
    int expected;
    unsigned int extra;
    unsigned char reserved132c[0x15b0 - 0x132c];
    unsigned char publish;
};
/* INTEGRATOR: move to include/game/service_runtime.h */
extern "C" {
extern void func_00182210(void);
extern void func_00182204(void);
extern int func_0018f018(void);
}

extern "C" int func_00148a9c(struct ServiceCoordinator *state, unsigned short value)
{
    func_00182210();
    if (!func_0018f018() || (value != 0 && value != 1)) {
        func_00182204();
        return 0;
    }
    struct CoordinatorRequest *request =
        state->completion_slots[2 + state->completion_slot];
    request->value = value;
    request->flags |= 0x40000000u;
    func_00182204();
    return 1;
}
