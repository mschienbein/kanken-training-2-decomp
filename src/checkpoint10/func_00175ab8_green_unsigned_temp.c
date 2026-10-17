#include "game/screen.h"
/* INTEGRATOR: differs from include/game/screen.h: stack-passed blue and alpha
 * are read as bytes, and the core color arguments are forwarded as unsigned char. */
struct DisplayPart;



struct DisplayColorState {
    struct DisplayData *object;
    unsigned char reserved04[0x10];
    unsigned char red, green, blue, alpha;
};
struct DisplayColorSlots {
    unsigned char reserved00[0x1300];
    struct DisplayColorState *states[];
};

extern void func_00170030(struct DisplayData *object, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha);
extern void func_00170088(struct DisplayData *object, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha);
extern void func_00170014(struct DisplayData *object, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha);
extern void func_0016fff8(struct DisplayData *object, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha);


static void apply_object_color(struct DisplayData *object, unsigned char green,
                               unsigned char red, unsigned char blue,
                               unsigned char alpha)
{
    if (object == 0)
        return;
    switch (func_0017a9a0(&object->base, 0)) {
    case 3:
        func_00170088(object, red, green, blue, alpha);
        break;
    case 5:
        func_00170014(object, red, green, blue, alpha);
        func_0016fff8(object, red, green, blue, alpha);
        break;
    case 6:
        func_00170030(object, red, green, blue, alpha);
        break;
    }
}

void func_00175ab8(struct DisplayColorSlots *owner, int index, unsigned char red,
                   unsigned char green, unsigned char blue, unsigned char alpha)
{
    unsigned green_value = green;
    struct DisplayColorState *state = owner->states[index];
    if (state != 0) {
        state->red = red;
        state->green = green_value;
        state->blue = blue;
        state->alpha = alpha;
        apply_object_color(state->object, green_value, red, blue, alpha);
    }
}
