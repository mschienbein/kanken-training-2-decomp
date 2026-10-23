#include "game/input_device.h"
#include "game/service_state.h"
/* [INFERENCE] The registered thread-entry callback has one context argument. */
void func_0012d104(void *storage)
{
    struct ThreadStartContext *start = storage;
    func_00121364();
    func_001213b0();
    func_0012d0b8();
    start->entry(start->argument, start->context);
    start->cleanup(start->context);
    func_0012e638();
    if (start->termination_argument != 0)
        func_0012d0c4(start->termination_argument);
    func_0012d148();
}
