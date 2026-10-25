#include "game/screen.h"
#include "game/selection.h"
#include "game/resource_runtime.h"

static void set_active(SelectorReference *reference, int active)
{
    if (!reference)
        return;
    ResourceEvent *object = reference->object;
    DisplayObject *display = object ? &object->base : 0;
    if (!display)
        return;
    switch (func_0017a9a0(display, 0)) {
    case 3: func_00195a08(display, 0, active); break;
    case 5: func_0015e948(display, active); break;
    case 6: func_001826fc(display, active); break;
    }
}

static void replaceDisplay(struct W6Selector *selector,
                                         struct SelectorReference *handle, int resource)
{
    if (handle != 0) {
        if (handle->object != 0) {
            func_00183208(handle->object);
            handle->object = 0;
        }
        handle = selector->handle;
        if (handle != 0 && handle->object == 0) {
            const struct NameTable *table = selector->resources[resource];
            struct ResourceEvent *object = func_0017a818(table, 3, 1, 0.0f, 0.0f, 1.0f);
            /* ORIGINAL BUG: label setup is not skipped if allocation fails. */
            if (table->count > 0)
                func_0017a968(&object->base, table, 0, 1);
            handle->object = object;
        }
    }
}

struct SelectorRequest { W6Selector *owner; int resource; };
static void replace_request(const SelectorRequest &request, SelectorReference *handle)
{
    replaceDisplay(request.owner, handle, request.resource);
}

void func_00174210(struct W6Selector *selector, int resource)
{
    struct SelectorReference *handle = selector->handle;
    if (resource < 0) {
        set_active(handle, 0);
    } else {
        SelectorRequest request;
        request.owner = selector;
        request.resource = resource;
        replace_request(request, handle);
    }
}
