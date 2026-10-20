#include "game/service_lock.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit.
 * In particular this does not assert a one-object-argument destructor ABI. */
void register_resource_service_lock_cleanup(struct StreamCriticalSection *object,
    void (*destroy)(void), void (*image_base)(void))
{
}
