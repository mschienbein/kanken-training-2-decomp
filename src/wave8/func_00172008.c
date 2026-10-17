#include "game/notification.h"

/* [INFERENCE] The payload's leading serialized storage has a UTF-16 view. */
unsigned short *func_00172008(int index)
{
    if (index == -1)
        return bss_00280af0.payload.storage.text;
    return bss_002476b0[index].storage.text;
}
