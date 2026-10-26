#ifndef GAME_SERVICE_LOCK_H
#define GAME_SERVICE_LOCK_H
#include "game/service_runtime.h"

struct ResourceServiceEvent {
    struct ResourceServiceEvent *previous;
    struct ResourceServiceEvent *next;
    void (*callback)(void *context);
    /* The dispatcher forwards this context unchanged; initialization stores null. */
    void *context;
    unsigned capacity;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct StreamCriticalSection *func_001135cc(struct StreamCriticalSection *lock);
extern void func_001c5530(void *context);
extern void func_001c51a0(void);
extern int func_00186424(struct StreamCriticalSection *lock);
extern void func_001863b0(struct StreamCriticalSection *lock);
extern CtrResult func_00148150(const CtrHandle *session, unsigned command,
                              void *buffer, unsigned size);
extern CtrResult func_00145e3c(unsigned command, void *buffer, unsigned size);
/* The erased registration does not store or invoke the no-argument cleanup. */
extern void register_resource_service_lock_cleanup(struct StreamCriticalSection *object,
    void (*destroy)(void), void (*image_base)(void));
#ifdef __cplusplus
}
#endif
#endif
