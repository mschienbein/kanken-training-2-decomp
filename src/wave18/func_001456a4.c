#include "game/functions.h"
#include <string.h>
#include "game/notification.h"




void func_001456a4(const struct NotificationPayload *input, int index, int reset)
{
    if (index >= 5)
        index -= 5;
    if (reset)
        func_00181250(index);
    else
        memcpy(func_00181224(index), input, sizeof(*input));
}
