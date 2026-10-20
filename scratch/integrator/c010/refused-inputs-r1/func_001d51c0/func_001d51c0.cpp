#include "game/scene_lifecycle.h"

struct SceneController {
    const void *vtable;
    unsigned char reserved04[1];
    unsigned char active;
    unsigned char reserved06[0x74];
    unsigned char enabled;
    unsigned char reserved7b[0xb5];
    SceneChild *children;
};

/* INTEGRATOR: move to include/game/scene_lifecycle.h */
extern "C" {
void func_0017db3c(SceneController *controller);
void func_0017daf8(SceneChild *child);
void func_0017dae0(SceneChild *child);
void func_00183758(SceneChild *child);
}

extern "C" void func_001d51c0(SceneController *controller, int mode)
{
    SceneChild *child;
    switch (mode) {
    case 0:
        controller->enabled = 0;
        return;
    case 1:
        controller->enabled = 1;
        return;
    case 3:
        for (child = controller->children; child != 0; child = child->next) {
            func_0017dae0(child);
            func_00183758(child);
        }
        break;
    case 2:
        func_0017db3c(controller);
        for (child = controller->children; child != 0; child = child->next) {
            if (child->active)
                func_0017daf8(child);
        }
        for (child = controller->children; child != 0; child = child->next)
            func_0017dae0(child);
        break;
    default:
        return;
    }
    controller->children = 0;
    controller->enabled = 1;
}
