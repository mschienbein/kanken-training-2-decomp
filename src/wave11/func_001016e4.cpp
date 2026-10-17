#include "game/sdk_init.h"
#include "game/runtime.h"
extern "C" void func_001016e4(void)
{
    Result result;
    func_00106f00(0x1600000);
    func_00106fbc(0x800000);
    result = Result(func_0019eef0(0x2200000));
    switch (result.failed()) {
    case 0:
        break;
    default:
        func_0019f568();
        break;
    }
}
