#include "game/functions.h"
#include "game/notification.h"

extern void func_00190e44(struct NotificationPayload *payload);

void func_00181e44(void)
{
    int index;
    func_00190e44(&bss_00280af0.payload);
    for (index = 0; index < 5; ++index)
        func_00190e44(&bss_002476b0[index]);
}
