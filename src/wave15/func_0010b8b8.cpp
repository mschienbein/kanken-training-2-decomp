#include "game/result.h"
#include "game/service_clients.h"



static __forceinline Result configure_client(void)
{
    Result result = func_00113c0c(&data_002430d8.handle, data_0021ac13);
    switch (result.failed()) {
    case 0:
        result = func_00113c74();
        switch (result.failed()) {
        case 0: return Result(0);
        default: break;
        }
        break;
    default: break;
    }
    return result;
}

static __forceinline Result initialize_client(void)
{
    Result result = func_00112558();
    switch (result.failed()) {
    case 0:
        result = configure_client();
        switch (result.failed()) {
        case 0: return Result(0);
        default: break;
        }
        break;
    default: break;
    }
    return result;
}

static __forceinline void check_result(Result result, unsigned address)
{
    switch (result.failed()) {
    case 0: break;
    default: func_00192afc((int)result.value, reinterpret_cast<const void *>(address));
    }
}

extern "C" void func_0010b8b8(void)
{
    if (!data_00242be0) {
        func_0010b98c();
        func_0019df5c();
        Result result = initialize_client();
        unsigned address = (unsigned)__current_pc();
        check_result(result, address);
        data_00242be0 = 1;
    }
}
