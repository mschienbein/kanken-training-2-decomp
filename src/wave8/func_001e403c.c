#include "game/text.h"

#include "game/screen.h"
struct Screen;
struct W8ChoiceController;
struct W8DisplaySet;
struct W8ResourceChoice;
struct W8EffectState;

struct W8ScreenState {
    unsigned char reserved00[0x14];
    struct W8EffectState *effects;
    unsigned char reserved18[0x34];
    struct W8ChoiceController *choices;
    struct W8DisplaySet *display;
    struct Screen *buttons;
    struct W8ResourceChoice *first_resource;
    struct W8ResourceChoice *second_resource;
    struct Screen *selector;
    struct Screen *other;
    unsigned char reserved68[0x2c];
    int phase;
    unsigned char reserved98[4];
    int previous_phase;
    unsigned char reserveda0[0x10];
    int text_length;
    unsigned char reservedb4[0x0c];
    unsigned short text[25];
    unsigned char reservedf2[0x8e];
    struct DisplayObject *cursor;
};

extern void func_0017a9ac(struct Screen *screen);
extern void func_00170e50(struct Screen *screen, int index);
extern void func_00179600(struct W8ChoiceController *controller, int index, int active);
extern void func_0017a0e0(struct W8DisplaySet *set, int index, int active);
extern void func_0017a6f4(struct Screen *screen, int index);
extern void func_00174210(struct W8ResourceChoice *resource, int index);
extern void func_00173d8c(struct W8ResourceChoice *resource, int index);
extern void func_001795cc(struct Screen *screen, int first, int second);
extern void func_0016de90(struct Screen *screen, int index);
extern void func_00178e98(struct W8ChoiceController *controller, int index);
extern void func_001e9818(void *screen, int index, int first, int second);
extern int func_00178f84(struct W8ChoiceController *controller, int index);
extern unsigned short *func_00172008(int index);
extern unsigned short *func_00178f28(unsigned short *destination, unsigned byte_limit,
                                   const unsigned short *source);
extern void func_001eafa0(void *screen, int index, const unsigned short *text);
extern void func_00181700(struct DisplayObject *object, float x, float y, float z);
extern void func_0015e684(struct DisplayObject *object, int index, float x, float y, float z);
extern void func_0016dc94(struct W8EffectState *effects, int index);

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
        func_00181700(object, object->position[0], object->position[1], object->position[2]);
        break;
    case 5:
        func_0015e684(object, 0, object->position[0], object->position[1], object->position[2]);
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
