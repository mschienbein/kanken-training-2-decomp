#include "ctr/svc.h"
#include "ctr/ipc.h"
#include "game/stream_manager.h"


#include "game/service_protocol.h"
extern int data_00243770;
extern CtrHandle data_002438a4;

extern const unsigned data_00220b5c;

static __forceinline void release_service(int *references)
{
    if (*references > 0)
        --*references;
    if (*references == 0) {
        int failure = (int)((unsigned)func_0012e7b4(&data_002438a4) & 0x80000000u);
        if (failure >= 0) {
            unsigned value = data_00220b5c;
            data_00243054 = value;
            data_002438a8.handle = value;
        }
    }
}

void func_0012159c(void)
{
    struct StreamMutex *mutex = func_0018f734();
    func_00192bd8(mutex);
    release_service(&data_00243770);
    func_00195d74(mutex);
}
