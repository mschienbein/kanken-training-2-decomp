#include "game/matrix.h"

static Vector3 uniform_scale(float value)
{
    Vector3 scale = { value, value, value };
    return scale;
}

TransformObjectW8C::TransformObjectW8C()
    : vtable(&data_00224470), scale(uniform_scale(1.0f)), translation()
{
    func_00195334(&matrix, matrix34Identity());
}
