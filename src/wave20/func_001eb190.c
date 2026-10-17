#include "game/screen.h"

struct TouchStrokeController {
    unsigned reserved00[3];
    struct BoundDisplayObject *objects[3];
    unsigned char reserved18[0x8a1];
    unsigned char active;
    unsigned char reserved8ba[0x0e];
    float origin_x;
    float origin_y;
};


extern void func_00166d5c(struct TouchStrokeController *controller, int enabled, float x, float y);

void func_001eb190(struct TouchStrokeController *controller)
{
    func_001826fc(&controller->objects[0]->base, 0);
    func_00180344();
    func_00166d5c(controller, 0, 0.0f, 0.0f);
    controller->active = 0;
}
