#include "game/notification.h"

unsigned char func_001dc100(int year, int month, int day)
{
    unsigned char enabled = 0;
    int key = func_00183070(year, month, day);
    int index;
    for (index = 0; index < 180; ++index) {
        if (bss_00280af0.payload.storage.fields.days[index].day == key) {
            enabled = bss_00280af0.payload.storage.fields.days[index].flags;
            break;
        }
    }
    return enabled;
}
