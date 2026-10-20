#include "game/functions.h"
#include <string.h>
#include "game/notification.h"


static inline int payload_index(int index)
{
    if (index >= 5)
        index -= 5;
    return index;
}

extern "C" void func_001457d4(NotificationPayload *output, int selection)
{
    int index = payload_index(selection);
    func_00181250(index);
    *output = *func_00181224(index);
}
