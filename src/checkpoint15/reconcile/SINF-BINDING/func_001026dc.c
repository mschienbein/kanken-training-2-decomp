/* [INFERENCE] Camera field/helper names. Hard-float sine/cosine identities
 * follow the adopted catalogue's complete licensed-library witnesses, not
 * the historical author's polynomial inference. */
#include <math.h>
#include "game/camera_defaults.h"

/* [INFERENCE] Euler-angle trigonometric cache; historical helper name unknown.
 * Roll is evaluated with the other angles, but does not change the forward axis. */
struct CameraTrigonometry {
    float sx, cx, sy, cy, sz, cz;
};

static void camera_trigonometry(struct CameraTrigonometry *t,
                                 const struct CameraVector *angles)
{
    t->sx = sinf(angles->x * 0.01745329238474369f);
    t->cx = cosf(angles->x * 0.01745329238474369f);
    t->sy = sinf(angles->y * 0.01745329238474369f);
    t->cy = cosf(angles->y * 0.01745329238474369f);
    t->sz = sinf(angles->z * 0.01745329238474369f);
    t->cz = cosf(angles->z * 0.01745329238474369f);
}

void func_001026dc(void)
{
    int i;
    struct CameraDefaults *camera = bss_00497720;
    for (i = 0; i < 8; ++i, ++camera) {
        camera->position.x = 0.0f;
        camera->position.y = 0.0f;
        camera->position.z = 0.0f;
        camera->target.x = 0.0f;
        camera->target.y = 0.0f;
        camera->target.z = 0.0f;
        camera->rotation.x = 0.0f;
        camera->rotation.y = 0.0f;
        camera->rotation.z = 0.0f;
        camera->up.x = 0.0f;
        camera->up.y = 1.0f;
        camera->up.z = 0.0f;
        camera->distance = 10.0f;
        camera->field_of_view = 45.0f;
        camera->width = 400.0f;
        camera->height = 240.0f;
        camera->top = 0.0f;
        camera->bottom = 240.0f;
        camera->left = 0.0f;
        camera->right = 320.0f;
        camera->near_plane = 1.0f;
        camera->far_plane = 1000.0f;
        camera->viewport_x = 0.0f;
        camera->viewport_y = 0.0f;
        camera->viewport_width = 400.0f;
        camera->viewport_height = 240.0f;
        camera->mode = 0;
        camera->screen_x = 0;
        camera->screen_width = 400;
        camera->screen_height = 240;
        camera->screen_y = 0;
        camera->flags = 4;
        camera->projection = 0;
        {
            struct CameraTrigonometry rotation;
            camera_trigonometry(&rotation, &camera->rotation);
            /* Forward is roll-invariant; keep the complete Euler evaluation. */
            camera->target.x = camera->position.x + camera->distance * (rotation.cx * rotation.sy);
            camera->target.y = camera->position.y - camera->distance * rotation.sx;
            camera->target.z = camera->position.z + camera->distance * (rotation.cx * rotation.cy);
            if (camera->rotation.x >= 90.0f && camera->rotation.x < 270.0f)
                camera->up.y = -1.0f;
            else
                camera->up.y = 1.0f;
        }
    }
    bss_004989c0.active = -1;
    bss_004989c0.previous = -1;
    bss_004989c0.blend = 0.0f;
}
