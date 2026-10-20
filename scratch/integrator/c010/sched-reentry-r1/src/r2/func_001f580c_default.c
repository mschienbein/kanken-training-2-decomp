#include "game/service_runtime.h"

struct ResourceServiceEvent {
    struct ResourceServiceEvent *previous;
    struct ResourceServiceEvent *next;
    void (*callback)(void *context);
    /* [INFERENCE] Opaque context: the actual dispatcher forwards this word
     * unchanged, and this provider initializes it to null and never reads it. */
    void *context;
    unsigned capacity;
};
/* 001135cc initializes lock->state and preserves the input receiver. The
 * event dispatcher at 00122d94 supplies event+0xc and discards the result;
 * 001c5530 ignores that input. The empty cleanup keeps its real no-arg type. */
extern struct StreamCriticalSection *func_001135cc(struct StreamCriticalSection *lock);
extern void func_001c5530(void *context);
extern void func_001c51a0(void);
extern void func_00100000(void);
extern void register_resource_service_lock_cleanup(struct StreamCriticalSection *object,
    void (*destroy)(void), void (*image_base)(void));

static void initialize_event(struct ResourceServiceEvent *event)
{
    event->previous = 0;
    event->next = 0;
    event->callback = func_001c5530;
    event->context = 0;
    event->capacity = 32768;
}

void func_001f580c(void)
{
    struct StreamCriticalSection *lock = func_001135cc(&data_00242b0c);
    /* [INFERENCE] The original registration call identity was erased. This
     * source-only typed helper neither stores nor invokes the no-argument
     * cleanup; no invocation-compatible destructor ABI is inferred. */
    register_resource_service_lock_cleanup(lock, func_001c51a0, func_00100000);
    data_00242b14 = 0;
    initialize_event(&bss_0049ef24);
}
