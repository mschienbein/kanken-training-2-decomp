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

int func_001dc400(int grade, int category, int rank, int major, int minor, int fine)
{
    short stored_rank = rank;
    int improved = 0;
    short *record = (short *)&bss_00280af8.storage.text[
        grade * 8 + category * 4 + 0x30ec];
    if (record[0] < stored_rank ||
        (record[0] == stored_rank && major * 100000 + minor * 1000 + fine <
         record[1] * 100000 + record[2] * 1000 + record[3])) {
        record[0] = stored_rank;
        record[1] = major;
        record[2] = minor;
        record[3] = fine;
        improved = 1;
    }
    return improved;
}

/* The shared serialized storage has the same packed-bit layout for clearing. */
void func_001dc538(int group, int item, int unused)
{
    unsigned char *row = result_storage()->payload.storage.fields.result_flags[group];
    int value = row[item / 2];
    int mask = (item & 1) ? 0x40 : 4;
    row[item / 2] = value & ~mask;
}
