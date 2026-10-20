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
extern const unsigned data_002248a4[];
void func_0017db3c(SceneController *controller);
void func_0017daf8(SceneChild *child);
void func_0017dae0(SceneChild *child);
}

/* INTEGRATOR: differs from src/wave4/func_001d54ac.c: the receiver has the
 * SceneController layout and the function returns that same receiver. */
extern "C" SceneController *func_001d54b0(SceneController *controller)
{
    controller->vtable = data_002248a4;
    func_0017db3c(controller);
    SceneChild *child;
    for (child = controller->children; child != 0; child = child->next) {
        if (child->active)
            func_0017daf8(child);
    }
    for (child = controller->children; child != 0; child = child->next)
        func_0017dae0(child);
    controller->children = 0;
    controller->active = 0;
    return controller;
}
