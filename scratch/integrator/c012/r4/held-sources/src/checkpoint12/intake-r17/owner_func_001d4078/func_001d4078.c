#include "game/archive_stream.h"

/* Resource constructors invoke this callback with an initialization result
 * and the registered writable owner context. [INFERENCE] Result-word spelling. */
void func_001d4078(unsigned result, struct ArchiveStreamObject *owner)
{
    if (owner) {
        struct StreamMutex *mutex = &owner->context.mutex;
        func_00192bd8(mutex);
        owner->context.result = result;
        func_00195d74(mutex);
    }
}
