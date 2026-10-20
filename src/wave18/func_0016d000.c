#include "game/functions.h"
#include "game/notification.h"

int func_0016d000(int index)
{
    const struct NotificationPayload *payload;
    if (index == -1)
        payload = &bss_00280af0.payload;
    else
        payload = &bss_002476b0[index];
    return payload->storage.words[14] != 0;
}
