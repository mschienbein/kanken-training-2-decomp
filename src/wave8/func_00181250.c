#include "game/notification.h"

void func_00181250(int index)
{
    struct NotificationPayload *input;
    if (index == -1)
        input = &bss_00280af0.payload;
    else
        input = &bss_002476b0[index];
    func_00190e44(input);
}
