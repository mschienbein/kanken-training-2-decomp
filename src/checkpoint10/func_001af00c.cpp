#include "game/screen.h"

struct MarkerPosition { short x, y; };
struct MarkerController {
    unsigned char reserved00[0x10];
    struct BoundDisplayObject **marker;
};
struct MarkerResources {
    unsigned char reserved00[0x14];
    const struct NameTable *labels;
};
/* INTEGRATOR: move to include/game/marker_controller.h */
extern "C" {
extern const MarkerPosition data_0023ed14[];
extern const MarkerPosition data_0023ed60[];
extern const MarkerPosition data_0023ede0[];
extern MarkerResources bss_0024716c;
void func_001701b4(MarkerController *controller, int index, int enabled);
void func_001af00c(MarkerController *controller, unsigned mode, int value, int total);
}

void func_001af00c(struct MarkerController *controller, unsigned mode, int value, int total)
{
    struct BoundDisplayObject *object = *controller->marker;
    const struct MarkerPosition *position;
    float fraction;
    int kind;
    if (!object || !total)
        return;
    fraction = (float)value / (float)total;
    switch (mode) {
    case 0: case 1:
        position = &data_0023ed14[(int)(fraction * 18.0f)];
        break;
    case 2: case 3:
        position = &data_0023ed60[(int)(fraction * 31.0f)];
        break;
    default:
        position = &data_0023ede0[(int)(fraction * 41.0f)];
        break;
    }
    func_0017a968(&object->base, bss_0024716c.labels, 0, 0);
    {
        float x = (float)position->x;
        float y = (float)position->y;
        object->base.position[0] = x;
        object->base.position[1] = y;
    }
    kind = func_0017a9a0(&object->base, 0);
    if (kind == 5)
        goto type5;
    if (kind == 6)
        goto type6;
    goto position_done;
type6:
    func_00181700(object, object->base.position[0], object->base.position[1], object->base.position[2]);
    goto position_done;
type5:
    func_0015e684(object, 0, object->base.position[0], object->base.position[1], object->base.position[2]);
position_done:
    func_001701b4(controller, 1, 1);
}
