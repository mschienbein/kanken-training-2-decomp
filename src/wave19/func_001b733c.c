struct TransformIdentity {
    unsigned char reserved00[0x24];
    float x, y, z, w;
};

/* [INFERENCE] The preserved object pointer is the initializer's return value;
 * there are no direct callers to establish the original spelling. */
void *func_001b733c(void *storage)
{
    struct TransformIdentity *transform = storage;
    transform->x = 0.0f;
    transform->y = 0.0f;
    transform->z = 0.0f;
    transform->w = 1.0f;
    return transform;
}
