#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Both queue clients use the two intrusive links of the canonical
 * display object. TU membership is a source hypothesis, not recovered identity. */
static struct BoundDisplayObject *next_object(struct BoundDisplayObject *object,
                                             int primary)
{
    if (primary)
        return object->next;
    return object->next_secondary;
}

void func_0018528c(struct BoundDisplayObject *object)
{
    struct DisplayObjectList *list = func_00196cb8(object->screen, object->index);
    if (list != 0) {
        struct BoundDisplayObject *previous = 0;
        struct BoundDisplayObject *current = list->head;
        if (object->queued == 0)
            return;
        object->queued = 0;
        while (current != 0 && current != object) {
            previous = current;
            current = next_object(current, 1);
        }
        if (current == 0)
            return;
        if (previous == 0) {
            /* ORIGINAL BUG: removing the head retains its outgoing link. */
            list->head = next_object(current, 1);
        } else {
            previous->next = next_object(current, 1);
            current->next = 0;
        }
    }
}
