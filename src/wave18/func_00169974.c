#include "game/functions.h"
#include "game/notification.h"




int func_00169974(void)
{
    int pending = bss_00290cac.pending;
    int allowed = pending != 2 && pending != 3 && pending != 4;
    if (!allowed)
        return 0;
    func_0019eb24();
    func_00181dcc(0, 0, allowed);
    return 1;
}
