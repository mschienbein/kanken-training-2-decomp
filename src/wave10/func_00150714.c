struct S150714 {
    unsigned char reserved[0x28];
    float value;
    unsigned char enabled;
};

void func_00150714(struct S150714 *object, float value, int enabled)
{
    object->value = value;
    object->enabled = enabled;
}
