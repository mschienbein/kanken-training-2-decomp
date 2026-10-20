#include "game/w11_control.h"

static __forceinline float lower_bound(float value, float minimum)
{
    return value > minimum ? value : minimum;
}
static __forceinline float upper_bound(float value, float maximum)
{
    return value > maximum ? maximum : value;
}
void func_001b3c0c(struct AnimationState *slider, int delta)
{
    if (slider->primary && slider->enabled) {
        slider->current += (float)delta;
        slider->current = lower_bound(slider->current, slider->lower);
        slider->current = upper_bound(slider->current, slider->upper);
        func_0016f288(slider, slider->current);
    }
}
