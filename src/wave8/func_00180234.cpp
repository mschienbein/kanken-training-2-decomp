#include "game/matrix.h"


struct Scale3fW8C {
    float x, y, z;
    Scale3fW8C(float value) : x(value), y(value), z(value) {}
};

struct TransformComponentsW8C {
    Matrix34 matrix;
    Scale3fW8C scale;
    unsigned flags;
    TransformComponentsW8C(unsigned initial_flags)
        : matrix(*matrix34Identity()), scale(1.0f), flags(initial_flags) {}
};

extern "C" TransformComponentsW8C *func_00180234(void)
{
    /* [INFERENCE] callee identity: the guard-release call was inlined by armlink. */
    static TransformComponentsW8C components_value(0x7e1);
    return &components_value;
}
