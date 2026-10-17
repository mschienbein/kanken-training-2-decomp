#include "game/notification.h"
#include "game/notification_selection.h"
#include "game/text_renderer.h"
void func_00173748(int secondary, int category, int index, int *value, int *extra)
{
    const struct NotificationSelection *table = &bss_00280af8.storage.selection;
    const struct W14ValuePair *pair;
    /* ORIGINAL BUG: negative category/index inputs are not checked. */
    if (!secondary)
        goto primary;
    if (index >= 50)
        return;
    pair = &table->secondary[category][index];
write_outputs:
    if (value)
        *value = pair->value;
    if (extra)
        *extra = pair->extra;
    return;

primary:
    if (index >= 150)
        return;
    pair = &table->primary[category][index];
    goto write_outputs;
}
