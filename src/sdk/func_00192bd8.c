#include "ctr/cp15.h"
#include "game/stream_manager.h"

extern void func_00136630(struct StreamCriticalSection *critical);

void func_00192bd8(struct StreamMutex *mutex)
{
    if (ctrGetThreadLocalStorage() != mutex->owner) {
        func_00136630(&mutex->critical);
        mutex->owner = ctrGetThreadLocalStorage();
    }
    ++mutex->depth;
}
