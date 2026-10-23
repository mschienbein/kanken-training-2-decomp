#include "game/framebuffer.h"


void func_0010d644(ViewportState *state, ViewportRenderTarget *target)
{
    float width = 0.0f;
    float height = 0.0f;
    if (target != 0) {
        width = (float)target->description()->width;
        height = (float)target->description()->height;
    }
    ViewportRectangle viewport;
    viewport.left = 0.0f;
    viewport.bottom = 0.0f;
    viewport.right = width;
    viewport.top = height;
    viewport.near_depth = 0.0f;
    viewport.far_depth = 1.0f;
    func_00116400(state, target, &viewport);
}
