#include "game/functions.h"
#include "game/notification.h"

struct NotificationPayload *func_00181224(int index)
{
    if (index == -1)
        return &bss_00280af0.payload;
    return &bss_002476b0[index];
}
