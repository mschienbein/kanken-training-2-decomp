#include <string.h>
#include "game/functions.h"
#include "game/service_runtime.h"
#include "game/object_manager.h"
#include "game/static_objects.h"

static void register_static_object(void *object, void (*image_base)(void),
                                    ObjectDestructor destroy);
static StreamMutex *initialize_mutex(StreamMutex *mutex);
static StreamCriticalSection *initialize_critical(StreamCriticalSection *critical,
                                                   unsigned initial_state);

void func_001f586c(void)
{
    memcpy(&data_00242be8, &data_0021ac34.handle, sizeof(data_00242be8));
    StreamMutex *mutex = initialize_mutex(&bss_004b1280);
    register_static_object(mutex, func_00100000, func_001c519c);
}

static void register_static_object(void *object, void (*image_base)(void),
                                    ObjectDestructor destroy)
{
    /* [INFERENCE] Registration identity at rewritten calls: the same retained
     * callee is visible in the final tail call and neighbouring init entries. */
    func_00164ebc(object, destroy, image_base);
}

static StreamMutex *initialize_mutex(StreamMutex *mutex)
{
    initialize_critical(&mutex->critical, 0);
    func_00195ec4(mutex);
    return mutex;
}


static StreamCriticalSection *initialize_critical(StreamCriticalSection *critical,
                                                   unsigned initial_state)
{
    critical->state = initial_state;
    return critical;
}
