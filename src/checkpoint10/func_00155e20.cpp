#include "game/service_runtime.h"
#include "ctr/svc.h"
#include "game/synchronization.h"
struct CoordinatorRequest {
    unsigned int flags;
    unsigned char reserved04[26];
    unsigned short value;
};
struct CoordinatorRequestBank {
    struct CoordinatorRequest *requests[24];
};
struct CoordinatorChannelSlots {
    struct CoordinatorRequest *reserved[6];
    struct CoordinatorRequestBank banks[2];
    struct CoordinatorRequest *trailing[96];
};
/* INTEGRATOR: differs from include/game/service_runtime.h: completion_slots
 * point to request records with flags at +0 and a value at +0x1e. */
struct ServiceCoordinator {
    CtrHandle ready_event;
    CtrHandle completion_event;
    struct StreamMutex mutex;
    unsigned char reserved14[0x109c - 0x14];
    union {
        struct CoordinatorRequest *completion_slots[150];
        struct CoordinatorChannelSlots channels;
    };
    struct ServiceBufferPair buffers[5];
    unsigned char enabled, pending;
    unsigned short serial, current_buffer, completion_slot;
    int expected;
    unsigned int extra;
    unsigned char reserved132c[0x15b0 - 0x132c];
    unsigned char publish;
};

extern "C" int func_00155e20(struct ServiceCoordinator *state, int channel)
{
    func_00182210();
    if (func_0018f018()) {
        state->channels.banks[state->serial & 1].requests[channel]->flags |= 16;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
