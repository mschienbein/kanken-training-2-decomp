#include "game/functions.h"
#include "game/notification.h"


extern void func_00192720(void *destination, const void *source, unsigned size);

void func_00172d48(int index)
{
    if (index == -1) {
        index = bss_00280af0.index;
        func_00192720(&bss_002476b0[index], &bss_00280af0.payload, sizeof(struct NotificationPayload));
    }
    func_0015d6e0(index);
    func_00169b58();
}
