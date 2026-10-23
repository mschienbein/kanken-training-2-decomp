#include "game/notification.h"

/* [INFERENCE] Local typed storage factoring, not a historical helper identity. */
static struct SavedNotification *result_storage(void)
{
    return &bss_00280af0;
}

/* Third caller-evidenced argument is unused; its role remains [INFERENCE]. */
void func_001dc3c4(int group, int item, int unused)
{
    unsigned char *row = result_storage()->payload.storage.fields.result_flags[group];
    int value = row[item / 2];
    int mask = (item & 1) ? 0x40 : 4;
    row[item / 2] = value | mask;
}


/* The shared serialized storage has the same packed-bit layout for clearing. */
void func_001dc538(int group, int item, int unused)
{
    unsigned char *row = result_storage()->payload.storage.fields.result_flags[group];
    int value = row[item / 2];
    int mask = (item & 1) ? 0x40 : 4;
    row[item / 2] = value & ~mask;
}
