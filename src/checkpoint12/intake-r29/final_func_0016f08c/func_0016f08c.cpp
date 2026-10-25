#include <string.h>
#include "game/input.h"
#include "nn/math.h"

/* The settings routines in the author TU are unrelated to this target;
 * only the actual tracking helper and coordinate snapshot are retained. */
static void remember_touch_reference(struct TouchMotion *touch)
{
    memcpy(&touch->previous, &touch->reference, sizeof(touch->previous));
}

/* [INFERENCE] History snapshot paired with a non-owning coordinate view. */
struct TrackingSnapshot {
    const InputPoint &position;
    float reference;
    explicit TrackingSnapshot(const TouchMotion &touch) : position(touch.sample.pointer.position)
    { memcpy(&reference, &touch.reference, sizeof(reference)); }
    void save_history(TouchMotion &touch) const
    { memcpy(&touch.previous, &reference, sizeof(reference)); }
};

int func_0016f08c(struct TouchMotion *touch)
{
    int result = 0;
    float x;
    if ((touch->sample.state & 4) || !(touch->sample.state & 1))
        goto finish;
    if (!(touch->sample.state & 2))
        goto begin_press;
    result = 2;
    if (touch->sample.pointer.released)
        result = 1;
    if (!touch->sample.pointer.released)
        goto track;
    touch->sample.state = 1;
finish:
    return result;

track:
    {
    TrackingSnapshot sample(*touch);
    nn::math::VEC2 position(sample.position.x, sample.position.y);
    sample.save_history(*touch);
    touch->reference = position.x;
    if (!touch->held && !func_001703c4(&touch->sample.rectangle, position.x, position.y)) {
        remember_touch_reference(touch);
        touch->sample.state = 1;
        result = 1;
    }
    goto finish;
    }

begin_press:
    if (touch->sample.pointer.active != 1)
        goto finish;
    x = touch->sample.pointer.position.x;
    if (!func_001703c4(&touch->sample.rectangle, x, touch->sample.pointer.position.y)) {
        remember_touch_reference(touch);
        touch->sample.state = 1;
    } else {
        touch->reference = x;
        touch->previous = x;
        touch->sample.state |= 2;
        result = 3;
    }
    goto finish;
}
