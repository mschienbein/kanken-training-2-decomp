#include "game/notification.h"

static __forceinline int readChannel(const struct NotificationFields *state,
                                    unsigned index)
{
    return index < 12 ? state->channels[index] : 0;
}

int func_001dbaac(unsigned index)
{
    return readChannel(&bss_00280af8.storage.fields, index);
}
