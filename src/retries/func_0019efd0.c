#include "game/screen.h"
#include "game/service_state.h"
int func_0019efd0(void)
{
    struct WorkQueue *const queue = &bss_004990d0;
    int count;
    if (queue->enabled) {
        if (queue->active) {
            count = queue->writeIndex - queue->readIndex;
            if (count < 0)
                count += 256;
            return count;
        }
        return 0;
    }
    if (queue->writeIndex == queue->readIndex)
        return 0;
    count = queue->writeIndex - queue->readIndex;
    if (count < 0)
        count += 256;
    return count;
}
