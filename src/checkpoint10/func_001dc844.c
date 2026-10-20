#include "game/notification.h"


int func_001dc844(int category, int index)
{
    const short *first;
    const short *second;
    int count = 0;
    int remaining;
    if (category == 0) {
        first = &bss_00280af8.storage.selection.primary[index][0].value;
        second = &bss_00280af8.storage.selection.primary[index][0].extra;
        remaining = 150;
        do {
            if (*first != -1 && *second != -1)
                ++count;
            first += 2;
            second += 2;
        } while (--remaining);
        return count;
    }
    first = &bss_00280af8.storage.selection.secondary[index][0].value;
    second = &bss_00280af8.storage.selection.secondary[index][0].extra;
    remaining = 50;
    do {
        if (*first != -1 && *second != -1)
            ++count;
        first += 2;
        second += 2;
    } while (--remaining);
    return count;
}
