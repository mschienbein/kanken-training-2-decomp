#include <string.h>
#include "game/notification.h"

extern struct NotificationPayload *func_00181224(int index);
extern void func_00181250(int index);

void func_001456a4(const struct NotificationPayload *input, int index, int reset)
{
    if (index >= 5)
        index -= 5;
    if (reset)
        func_00181250(index);
    else
        memcpy(func_00181224(index), input, sizeof(*input));
}
