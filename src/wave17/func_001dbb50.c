#include "game/notification.h"

static __forceinline void writeChannel(struct NotificationFields *state,
                                      unsigned index, signed char value)
{
    if (index < 12)
        state->channels[index] = value;
}

void func_001dbb50(unsigned index, signed char value)
{
    writeChannel(&bss_00280af8.storage.fields, index, value);
}
