#include "game/service_state.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"


static __inline unsigned int ctrIpcStaticBufferDescriptor(unsigned int size,
                                                          unsigned int index)
{
    unsigned int descriptor = index << 10;
    descriptor |= size << 14;
    return descriptor | 2u;
}

unsigned func_001234e4(const void *buffer, unsigned size, unsigned value)
{
    unsigned *command = (unsigned *)(ctrGetThreadLocalStorage() + 0x80);
    unsigned result;
    unsigned handle_descriptor = 0;

    command[0] = 0x002c0044u;
    command[1] = size;
    command[2] = handle_descriptor;
    command[3] = value;
    command[4] = ctrIpcStaticBufferDescriptor(size, 0);
    command[5] = (unsigned)buffer;
    result = (unsigned)svcSendSyncRequest(data_00243040);
    if ((int)(result & 0x80000000u) >= 0)
        result = command[1];
    return result;
}
