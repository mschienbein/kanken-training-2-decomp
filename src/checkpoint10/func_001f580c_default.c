#include "game/service_runtime.h"

struct ResourceServiceEvent {
    struct ResourceServiceEvent *previous;
    struct ResourceServiceEvent *next;
    void (*callback)(void);
    unsigned position;
    unsigned capacity;
};
extern struct StreamCriticalSection *func_001135cc(struct StreamCriticalSection *lock);
extern void func_001c5530(void);
/* INTEGRATOR: differs from src/wave3/func_001c51a0.c: the registered callback receives the lock object. */
extern void func_001c51a0(struct StreamCriticalSection *lock);

static void initialize_event(struct ResourceServiceEvent *event)
{
    event->previous = 0;
    event->next = 0;
    event->callback = func_001c5530;
    event->position = 0;
    event->capacity = 32768;
}

void func_001f580c(void)
{
    struct StreamCriticalSection *lock = func_001135cc(&data_00242b0c);
    /* [INFERENCE] Destructor-registration identity: retained empty callee inlined by armlink. */
    func_00216140(lock, (ObjectDestructor)func_001c51a0, func_00100000);
    data_00242b14 = 0;
    initialize_event(&bss_0049ef24);
}
