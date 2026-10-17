#include "game/screen.h"
#include "game/material.h"
#include "game/matrix.h"

struct UpdateVec2 { float x, y; };
struct PendingDisplayUpdate {
    BoundDisplayObject *object;
    Vector3 position;
    Vector3 rotation;
    UpdateVec2 scale;
    unsigned char reserved24[4];
    int screen;
    unsigned dirty;
    unsigned char reserved30[4];
    UpdateVec2 size;
    DisplayTexCoords coordinates;
    unsigned char color[4];
    unsigned char reserved60[8];
    int animation;
};
/* INTEGRATOR: move to include/game/display_transform.h */
extern "C" {
void func_001ebca0(void *owner, PendingDisplayUpdate *update);
void func_001815a8(BoundDisplayObject *object, int screen, int index);
void func_001816c0(BoundDisplayObject *object, float x, float y, float z);
void func_0014d000(BoundDisplayObject *object, const DisplayTexCoords *coordinates);
void func_00181620(BoundDisplayObject *object, unsigned char red,
                   unsigned char green, unsigned char blue, unsigned char alpha);
}
static inline void set_size(BoundDisplayObject *object, UpdateVec2 size)
{
    func_00181734(object, size.x, size.y);
}
/* [INFERENCE] The unused first argument is the owning update context. */
void func_001ebca0(void *owner, PendingDisplayUpdate *update)
{
    if (update->dirty) {
        if (update->dirty & 0x100)
            func_001815a8(update->object, update->screen, 0);
        if (update->dirty & 0x80)
            func_00181100(update->object, update->animation, 0);
        if (update->dirty & 0x40)
            set_size(update->object, update->size);
        if (update->dirty & 1) {
            const Vector3 *value = &update->position;
            func_00181700(update->object, value->x, value->y, value->z);
        }
        if (update->dirty & 8) {
            const UpdateVec2 *value = &update->scale;
            func_001816e0(update->object, value->x, value->y);
        }
        if (update->dirty & 4) {
            const Vector3 *value = &update->rotation;
            func_001816c0(update->object, value->x, value->y, value->z);
        }
        if (update->dirty & 0x20)
            func_0014d000(update->object, &update->coordinates);
        if (update->dirty & 0x10) {
            const unsigned char *color = update->color;
            func_00181620(update->object, color[0], color[1], color[2], color[3]);
        }
        update->dirty = 0;
    }
}
