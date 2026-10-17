#include "game/functions.h"
#include "game/notification.h"

int func_00174430(void)
{
    /* [INFERENCE] The byte at payload offset 0x48 is a saved boolean flag. */
    return bss_00280af0.payload.storage.fields.reserved36[0x12] & 1;
}
