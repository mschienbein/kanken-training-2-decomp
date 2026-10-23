#include "ctr/cp15.h"
#include "game/synchronization.h"

struct RecursiveMutex {
    int state;
    unsigned char *owner;
    unsigned int depth;
};


int func_0010be14(struct RecursiveMutex *mutex)
{
    if (ctrGetThreadLocalStorage() != mutex->owner) {
        if (!func_0011359c(&mutex->state))
            return 0;
        mutex->owner = ctrGetThreadLocalStorage();
    }
    ++mutex->depth;
    return 1;
}
