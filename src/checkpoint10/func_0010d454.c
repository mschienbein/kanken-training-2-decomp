#include "game/applet.h"
#include <stdbool.h>
#include "ctr/svc.h"
#include "game/service_state.h"

struct AppletTimeout {
    long long nanoseconds;
};
extern struct AppletTimeout data_00242a40;


extern CtrResult func_00115ad4(unsigned state);



extern bool func_00191e18(unsigned *handle, void *buffer, unsigned capacity,
                           void **memory, unsigned *size, struct AppletTimeout timeout);





static __forceinline void check_result(CtrResult result, const void *location)
{
    unsigned int failure = (unsigned int)result >> 31;
    if (failure != 0)
        func_00192afc(result, location);
}

void func_0010d454(bool notify)
{
    CtrResult result;
    if (notify)
        func_00195e64(false);
    func_0019ecf8();
    result = func_00115ad4(func_00191de8());
    check_result(result, (const void *)__current_pc());
    func_0019ec88();
    if (func_00191dbc() && !(func_00191de8() & 0x20)) {
        unsigned handle;
        void *memory;
        bool available;
        func_00191dac(0x62);
        available = func_00191e18(&handle, func_0011561c(), 0x1000,
                                   &memory, 0, data_00242a40);
        func_0011587c(handle);
        func_001155f8(memory);
        func_00115608();
        func_00115628(available);
    }
}
