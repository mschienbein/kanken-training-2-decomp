#include "game/notification.h"

void func_001dbf40(unsigned *value, unsigned *minutes, unsigned *seconds, unsigned *milliseconds)
{
    if (value)
        *value = bss_00280af0.payload.storage.words[0xf];
    if (minutes)
        *minutes = bss_00280af0.payload.storage.words[0x10] / 60000;
    if (seconds)
        *seconds = (bss_00280af0.payload.storage.words[0x10] / 1000) % 60;
    if (milliseconds)
        *milliseconds = bss_00280af0.payload.storage.words[0x10] % 1000;
}
