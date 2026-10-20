#include "game/drawing.h"
#include "game/selection.h"
#include "game/animation.h"

struct SelectionModes {
    unsigned char reserved00[0x90];
    int active;
    int modes[7];
};


void func_00171a98(struct W6SelectionScreen *screen, int use_active)
{
    float x = 0.0f;
    float y = 0.0f;
    float maximum;
    int list;
    const int mode = use_active
        ? ((const struct SelectionModes *)screen)->modes[
              ((const struct SelectionModes *)screen)->active]
        : ((const struct SelectionModes *)screen)->modes[0];
    if (mode == 2) {
        maximum = 1056.0f;
        list = 1;
    } else if (mode == 3) {
        list = func_00171c7c(screen);
        maximum = (func_00171c44(screen->list, list) - 1) * 96.0f;
    } else {
        return;
    }
    func_00178fb0((const struct AnimationOwner *)screen->list, list, &x, &y);
    if (func_00174324((const struct AnimationOwner *)screen->list, 16)) {
        if (x <= 0.0f)
            func_00179600(screen->list, 16, 0);
    } else {
        if (x > 0.0f)
            func_00179600(screen->list, 16, 1);
    }
    if (func_00174324((const struct AnimationOwner *)screen->list, 17)) {
        if (x >= maximum)
            func_00179600(screen->list, 17, 0);
    } else {
        if (x < maximum)
            func_00179600(screen->list, 17, 1);
    }
}
