#include "game/functions.h"
#include "game/service_state.h"


int func_00123398(void)
{
    if (data_00242a28.pending == 0)
        return 0;
    data_00242a28.pending = 0;
    return func_0012f980();
}
