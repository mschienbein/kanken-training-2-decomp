#include "game/input.h"
#include "game/selection.h"

struct SliderState {
    unsigned char reserved00[0x10];
    void *primary;
    unsigned char reserved14[0x40];
    int enabled;
    float minimum;
    float maximum;
    float value;
};

extern struct SliderState *func_00167284(void);
extern void func_001b3c0c(struct SliderState *slider, int delta);
extern int func_0018ffac(int limit);

void func_001e74b8(struct W6SelectionScreen *screen)
{
    struct SliderState *slider = func_00167284();
    if (slider) {
        if (bss_004972d8.direction & 0x800)
            func_001b3c0c(slider, 1);
        else if (bss_004972d8.direction & 0x400)
            func_001b3c0c(slider, -1);
    }
    if (!screen->list || !(bss_004972d8.buttons & 0x200000))
        return;
    if (func_0018ffac(2)) {
        func_00179600(screen->list, 0, 0);
        func_00179600(screen->list, 1, 1);
    } else {
        func_00179600(screen->list, 0, 1);
        func_00179600(screen->list, 1, 0);
    }
}
