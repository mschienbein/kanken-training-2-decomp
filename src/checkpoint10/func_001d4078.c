#include "game/stream_manager.h"

struct ResultCallbackOwner {
    unsigned char reserved00[0x180];
    unsigned result;
    struct StreamMutex mutex;
};

/* Resource constructors invoke this callback with an initialization result
 * and the registered writable owner context. [INFERENCE] Result-word spelling. */
void func_001d4078(unsigned result, struct ResultCallbackOwner *owner)
{
    if (owner) {
        struct StreamMutex *mutex = &owner->mutex;
        func_00192bd8(mutex);
        owner->result = result;
        func_00195d74(mutex);
    }
}
