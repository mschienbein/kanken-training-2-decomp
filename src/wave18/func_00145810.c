#include <string.h>
#include "game/notification.h"

extern struct NotificationPayload *func_00181224(int index);

void func_00145810(struct NotificationPayload *output, int index)
{
    if (index >= 5)
        index -= 5;
    memcpy(output, func_00181224(index), sizeof(*output));
}
