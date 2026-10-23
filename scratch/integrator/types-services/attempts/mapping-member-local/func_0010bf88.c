#include "game/shared_memory.h"

CtrResult func_0010bf88(void)
{
    struct SharedMemory *address;
    CtrHandle *handle = &data_00242d88.handle;
    CtrResult result = func_001151b8(handle, &address);
    unsigned int failure = (unsigned int)result >> 31;
    if (failure)
        return result;
    result = svcMapMemoryBlock(*handle, 0, 1, 0x10000000);
    failure = (unsigned int)result >> 31;
    if (failure)
        return result;
    data_00242d88.address = address;
    return 0;
}
