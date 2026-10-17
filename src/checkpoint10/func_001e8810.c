#include <stddef.h>
#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Partial dynamic display-list and value-member layouts. */
struct W24Pair { float x, y; };
struct W24Float4 { float x, y, width, height; };
struct W24Transform { float values[9]; };
struct W24ElementExtra;
struct W24CollectionElement {
    struct DisplayObject *object;
    unsigned int flags;
    unsigned short reserved08;
    short index;
    unsigned short id;
    unsigned char mode, active, visible, pending;
    unsigned short count;
    void *user_data;
    float progress;
    struct W24Pair position, extent;
    struct W24ElementExtra *extra;
};
struct W24CollectionState {
    unsigned int kind;
    int count;
    struct W24CollectionElement **elements;
    struct W24Transform transform;
    struct W24Float4 bounds, viewport;
    struct W24Pair pairs[5];
    int active;
    int selected;
    unsigned int reserved80;
    struct MotionState motion;
};
struct W24CollectionScreen {
    unsigned char reserved00[0x20];
    struct W24CollectionState *states[20];
};
extern void func_00183208(struct DisplayObject *object);
extern void func_0019e08c(void *allocation);
extern void func_0016f024(struct MotionState *state);
extern void func_001e9ea4(struct W24CollectionScreen *screen, int index);

void func_001e8810(struct W24CollectionScreen *screen, int index)
{
    struct W24CollectionState *state = screen->states[index];
    if (state != 0) {
        state->selected = -1;
        if (state->elements != 0) {
            int i;
            for (i = 0; i < state->count; ++i) {
                struct W24CollectionElement *element = state->elements[i];
                if (element != 0) {
                    void *extent;
                    struct W24Pair *position;
                    if (element->object != 0) func_00183208(element->object);
                    if (element->extra != 0) {
                        /* [INFERENCE] callee identity: inlined by armlink */
                        func_001852f4(func_0018f7f0(element->extra));
                    }
                    element->object = 0;
                    element->flags = 0;
                    element->index = -1;
                    element->mode = 0;
                    element->id = 0;
                    element->active = 0;
                    element->visible = 0;
                    element->pending = 0;
                    element->count = 0;
                    element->position.x = 0.0f;
                    element->position.y = 0.0f;
                    element->extent.x = 0.0f;
                    element->extent.y = 0.0f;
                    element->progress = 0.0f;
                    element->user_data = 0;
                    element->extra = 0;
                    /* [INFERENCE] callee identity: inlined by armlink */
                    extent = func_0018f7f0(&element->extent);
                    position = (struct W24Pair *)((char *)extent -
                        (offsetof(struct W24CollectionElement, extent) - offsetof(struct W24CollectionElement, position)));
                    /* [INFERENCE] callee identity: inlined by armlink */
                    position = (struct W24Pair *)func_0018f7f0(position);
                    element = (struct W24CollectionElement *)((char *)position - offsetof(struct W24CollectionElement, position));
                    func_001852f4(element);
                }
            }
            func_0019e08c(state->elements);
        }
        func_0016f024(&state->motion);
        func_001e9ea4(screen, index);
    }
}
