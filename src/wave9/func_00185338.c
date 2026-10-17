#include <math.h>

struct Vector3 { float x, y, z; };

struct Vector3 *func_00185338(struct Vector3 *out, const struct Vector3 *in, const struct Vector3 *fallback)
{
    float x = in->x;
    float y = in->y;
    float z = in->z;
    const float length_squared = x * x + y * y + z * z;
    if (length_squared != 0.0f) {
        float inverse_length = 1.0f / sqrtf(length_squared);
        x *= inverse_length;
        y *= inverse_length;
        z *= inverse_length;
        out->x = x;
        out->y = y;
        out->z = z;
    } else {
        *out = *fallback;
    }
    return out;
}
