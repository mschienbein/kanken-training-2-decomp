#include "game/matrix.h"

struct MatrixIdentityW8C : Matrix34 {
    MatrixIdentityW8C()
    {
        elements[0] = 1.0f; elements[1] = 0.0f; elements[2] = 0.0f; elements[3] = 0.0f;
        elements[4] = 0.0f; elements[5] = 1.0f; elements[6] = 0.0f; elements[7] = 0.0f;
        elements[8] = 0.0f; elements[9] = 0.0f; elements[10] = 1.0f; elements[11] = 0.0f;
    }
};

__forceinline const Matrix34 *matrix34Identity()
{
    /* [INFERENCE] callee identity: the guard-release call was inlined by armlink. */
    static MatrixIdentityW8C identity_value;
    return &identity_value;
}

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
