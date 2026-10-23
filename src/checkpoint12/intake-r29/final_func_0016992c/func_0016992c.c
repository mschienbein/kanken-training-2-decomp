#include "game/notification.h"
#include "game/item_text.h"

static struct SavedNotification *result_storage(void)
{
    return &bss_00280af0;
}

static const unsigned char *result_row(int group)
{
    return result_storage()->payload.storage.fields.result_flags[group];
}

static unsigned required_result_bits(int index, unsigned mask)
{
    if (index & 1)
        mask = 0xf0 & (mask << 4);
    else
        mask &= 0xf;
    return mask;
}

int func_0016992c(int group, int index, int mask, int value)
{
    const unsigned char *packed =
        &result_row(group)[index / 2];
    unsigned bits = required_result_bits(index, mask);
    int result = 0;
    if ((bits & ~*packed) == 0)
        result = 1;
    return result;
}
