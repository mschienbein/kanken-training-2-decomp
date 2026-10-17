struct Vector3 { float x, y, z; };

struct Vector3 *func_0018346c(struct Vector3 *out, const struct Vector3 *a, const struct Vector3 *b)
{
    float x = a->y * b->z - a->z * b->y;
    float y = a->z * b->x - a->x * b->z;
    float z = a->x * b->y - a->y * b->x;
    out->x = x;
    out->y = y;
    out->z = z;
    return out;
}
