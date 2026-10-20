#include "game/functions.h"
#include "game/animation.h"

struct AnimationCurve {
    float start;
    float finish;
    int total;
    int current;
};
struct AnimatedObject;
struct AnimatedObjectVtable {
    void (*reserved00[4])();
    void (*idle)(AnimatedObject *object);
    void (*reserved14[4])();
    void (*started)(AnimatedObject *object);
};
struct AnimatedObject {
    const AnimatedObjectVtable *vtable;
    unsigned char reserved04[0x0c];
    AnimationOwner *owner;
    unsigned char reserved14[0x54];
    AnimationCurve curve;
    unsigned char reserved78[0x20];
    unsigned char enabled;
    unsigned char secondary_enabled;
    bool active;
    bool transitioning;
    unsigned char reserved9c;
    bool paused;
    unsigned char reserved9e[3];
    unsigned char mode;
    bool flag_a2;
    bool flag_a3;
    unsigned char reserveda4[0x5c];
    PoolLink update_link;
    PoolLink owner_link;
};

/* INTEGRATOR: move to include/game/animation.h */
extern "C" {
extern float func_00182aa8(const AnimationCurve *curve);
extern void func_00143328(AnimationOwner *owner, AnimatedObject *object);
}

static inline void set_curve(AnimationCurve *curve, float finish, int total)
{
    curve->start = func_00182aa8(curve);
    curve->finish = finish;
    curve->total = total;
    curve->current = 0;
}

static inline bool is_enabled(const AnimatedObject *object)
{
    if (object->enabled)
        return true;
    return object->secondary_enabled;
}

extern "C" void func_00183f24(AnimatedObject *object, int frames)
{
    if (frames > 0 && object->mode != 2 && is_enabled(object)) {
        int total = (int)(func_00182aa8(&object->curve) * frames);
        set_curve(&object->curve, 0.0f, total);
        object->paused = 0;
        if (object->owner != 0)
            func_00143328(object->owner, object);
        object->vtable->started(object);
        object->active = 0;
        object->mode = 0;
        object->flag_a3 = 0;
        object->transitioning = 1;
        object->flag_a2 = 0;
    } else {
        object->vtable->idle(object);
    }
}
