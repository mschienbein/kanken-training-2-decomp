#include "game/text.h"

#include "game/screen.h"
#include "game/selection.h"
#include "game/notification.h"
#include "game/choice_screen.h"


static __forceinline void set_active(struct DisplayObject *object, int active)
{
    if (object != 0) {
        switch (func_0017a9a0(object, 0)) {
        case 3: func_00195a08(object, 0, active); break;
        case 6: func_001826fc(object, active); break;
        case 5: func_0015e948(object, active); break;
        }
    }
}

static __forceinline void set_position(struct DisplayObject *object, float x, float y)
{
    /* ORIGINAL BUG: entering this phase assumes the cursor object exists. */
    object->position[0] = x;
    object->position[1] = y;
    switch (func_0017a9a0(object, 0)) {
    case 6:
        func_00181700((struct BoundDisplayObject *)object, object->position[0], object->position[1], object->position[2]);
        break;
    case 5:
        func_0015e684((struct BoundDisplayObject *)object, 0, object->position[0], object->position[1], object->position[2]);
        break;
    }
}

void func_001e403c(struct W8ScreenState *state)
{
    if (state->previous_phase == 1) {
        func_0017a9ac(state->buttons);
        func_00170e50(state->selector, 17);
        func_00170e50(state->selector, 10);
        func_00179600(state->choices, 4, 0);
        func_00179600(state->choices, 5, 0);
        func_0017a0e0(state->display, 2, 0);
        set_active(state->cursor, 0);
    }
    if (state->phase == 1) {
        struct DisplayObject *cursor;
        int length;
        float x, y;
        func_0017a6f4(state->buttons, 12);
        func_00174210(state->first_resource, 123);
        func_00173d8c(state->second_resource, 40);
        func_001795cc(state->selector, 17, 10);
        func_0016de90(state->other, 5);
        func_00178e98(state->choices, 5);
        func_00179600(state->choices, 4, 1);
        func_00179600(state->choices, 5, 1);
        func_001e9818(state->choices, 4, 2, 1);
        func_00178f84(state->choices, 4);
        func_00178f28(state->text, 0x30, func_00172008(-1));
        state->text_length = func_0017adc4(state->text);
        func_001eafa0(state->display, 2, state->text);
        length = state->text_length;
        if (length == 10)
            length = 9;
        cursor = state->cursor;
        y = -20.0f;
        x = (float)length * 26.5f - 124.0f;
        set_position(cursor, x, y);
        set_active(state->cursor, 1);
        func_0016dc94(state->effects, 0);
    }
}
