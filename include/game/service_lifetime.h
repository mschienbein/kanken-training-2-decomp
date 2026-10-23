#ifndef GAME_SERVICE_LIFETIME_H
#define GAME_SERVICE_LIFETIME_H

struct Result;
struct ScopedServiceLock;
struct StreamMutex;

#include "ctr/svc.h"
#include "game/runtime.h"
#include "game/stream_manager.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
class ScopedServiceLock {
    StreamMutex *mutex;
public:
    ScopedServiceLock(StreamMutex *object) : mutex(object) { func_00192bd8(mutex); }
    ~ScopedServiceLock() { func_00195d74(mutex); }
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
CtrResult func_0012d478(void);
extern struct Result func_0010b804(void);
extern unsigned func_0013e7d0(void);
extern CtrResult func_00136d4c(void);
extern CtrResult func_00136278(void);
extern CtrResult func_0012d3e4(void);
#ifdef __cplusplus
}
#endif

#endif
