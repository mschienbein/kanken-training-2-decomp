#include "game/screen.h"
#include "game/selection.h"
#include "game/pane_descriptors.h"
#include <string.h>


inline void set_active(struct DisplayObject *object, int enabled)
{
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3: return func_00195a08(object, 0, enabled);
    case 5: return func_0015e948(object, enabled);
    case 6: return func_001826fc(object, enabled);
    }
}

extern "C" void func_001b5b60(struct Screen *screen, int index, int text_index)
{
    struct WidgetSlot *slot = screen->slots[index];
    struct BoundDisplayObject *object;
    const struct PanePlacement *position;
    float x, y;
    if (!slot || !(object = (struct BoundDisplayObject *)slot->object))
        return;
    position = bss_00246f7c[index];
    x = position->x;
    if (!func_00179148())
        x = -x;
    memcpy(&y, &position->y, sizeof(y));
    object->base.position[0] = x;
    memcpy(&object->base.position[1], &y, sizeof(y));
    switch (func_0017a9a0(&object->base, 0)) {
    case 6:
        func_00181700(object, object->base.position[0], object->base.position[1], object->base.position[2]);
        break;
    case 5:
        func_0015e684(object, 0, object->base.position[0], object->base.position[1], object->base.position[2]);
        break;
    }
    func_0017a968(slot->object, bss_00246f4c[index], text_index, 0);
    set_active(slot->object, 1);
}
