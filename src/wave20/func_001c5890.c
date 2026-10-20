struct IndexedTransform {
    unsigned int index;
    unsigned char reserved04[0x30];
    float matrix[3][4];
    unsigned int reserved64;
    struct LockedTransform *transform;
};

struct IndexedTransform *func_001c5890(struct IndexedTransform *object,
                                     unsigned int index)
{
    object->index = index;
    object->matrix[0][0] = object->matrix[1][0] = object->matrix[2][0] = 0.0f;
    object->matrix[0][1] = object->matrix[1][1] = object->matrix[2][1] = 0.0f;
    object->matrix[0][2] = object->matrix[1][2] = object->matrix[2][2] = 0.0f;
    object->matrix[0][3] = object->matrix[1][3] = object->matrix[2][3] = 0.0f;
    return object;
}
