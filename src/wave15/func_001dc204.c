#include "game/notification.h"

extern int func_001699b8(void);

/* The reset caller explicitly passes its mode; this callback ignores it. */
void func_001dc204(int mode)
{
    int index = func_001699b8();
    unsigned char *flags = &bss_00280af0.payload.storage.fields.days[index].flags;
    *flags |= 1;
}
