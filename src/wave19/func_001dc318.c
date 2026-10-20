#include "game/notification.h"

int func_001dc318(int index)
{
    const struct NotificationPayload *payload;
    if (index == -1)
        payload = &bss_00280af8;
    else
        payload = &bss_002476b0[index];
    return (short)payload->storage.text[0x18];
}
