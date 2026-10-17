#include "game/matrix.h"

struct FrustumBounds {
    float left, right, bottom, top, near_plane, far_plane;
};

void func_00114bac(struct FrustumBounds *bounds, const struct Matrix44 *projection)
{
    const float *m = projection->elements;
    float scale;
    bounds->near_plane = m[11] / m[10];
    bounds->far_plane = m[11] / (m[10] - 1.0f);
    scale = m[11] / (m[0] * m[10]);
    bounds->left = (m[2] - 1.0f) * scale;
    bounds->right = (m[2] + 1.0f) * scale;
    scale = m[11] / (m[5] * m[10]);
    bounds->top = (m[6] + 1.0f) * scale;
    bounds->bottom = (m[6] - 1.0f) * scale;
}
