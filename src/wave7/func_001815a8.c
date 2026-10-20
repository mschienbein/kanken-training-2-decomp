#include "game/screen.h"
#include "game/runtime.h"

void func_001815a8(struct DisplayObject *object, int screen, int index)
{
    struct DisplayObjectList *list;
    struct DisplayObject *current;
    struct DisplayObject *previous;
    object->screen = screen;
    object->index = index;
    list = func_00196cb8(screen, index);
    /* ORIGINAL BUG: a null lookup is dereferenced before testing queued. */
    current = list->head;
    previous = 0;
    if (!object->queued) {
        while (current && current->position[2] < object->position[2]) {
            previous = current;
            current = current->next;
        }
        object->next = current;
        if (previous == 0)
            list->head = object;
        else
            previous->next = object;
        object->queued = 1;
    }
}
