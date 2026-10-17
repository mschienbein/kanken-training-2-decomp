#include <stdbool.h>
#include "ctr/svc.h"
#include "game/runtime.h"
#include "game/stream_manager.h"



static __inline bool result_failed(unsigned result)
{
    union {
        unsigned value;
        struct {
            unsigned detail : 31;
            unsigned failed : 1;
        } fields;
    } decoded;
    decoded.value = result;
    return decoded.fields.failed;
}

void func_001217b8(void)
{
    struct StreamMutex *lock = func_0018f734();
    func_00192bd8(lock);
    if (data_00243048.references > 0)
        --data_00243048.references;
    if (data_00243048.references == 0 && data_00243048.initialized) {
        data_00243048.initialized = 0;
        if (data_00243054) {
            unsigned result = svcCloseHandle(data_00243054);
            if (result_failed(result))
                func_0019f568();
            data_00243054 = data_0021c5d0;
        }
    }
    func_00195d74(lock);
}
