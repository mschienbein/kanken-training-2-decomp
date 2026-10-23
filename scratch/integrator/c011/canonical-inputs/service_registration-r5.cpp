#include "ctr/ipc.h"
#include "game/applet.h"
#include "game/service_state.h"
#include "game/result.h"

/* [INFERENCE] Source-local locked notification operation. */
static CtrResult notify_registration(unsigned char value)
{
    func_0019ecf8();
    CtrResult result = func_0010d418(value);
    func_0019ec88();
    return result;
}
unsigned func_00101714(unsigned flags)
{
    if (func_0010d0c0() != 0)
        return 0;
    flags &= ~7u;
    /* The +3 shared flag is used here as registration-started state. */
    data_00242a28.graphics_active = 1;
    func_0019ed2c(&bss_0049ee7c, 0x100);
    Result result(func_0010d0d0(0x300, flags, 0xf));
    if (ctrIpcTransportFailure(static_cast<CtrResult>(result.value)) < 0)
        return result.value;
    notify_registration(0);
    return 0;
}
