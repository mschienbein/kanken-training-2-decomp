#include "game/notification.h"

void func_00145658(unsigned status)
{
    if (bss_00290cac.status == 0) {
        bss_00290cac.status = status;
        if (status != 0) {
            bss_00290cac.pending = 0;
            bss_00290cac.value = 0;
            bss_00290cac.enabled = 0;
            bss_00290cac.acknowledged = 0;
        }
    }
}
