struct S150720 {
    unsigned char reserved[0x38];
    float values[];
};

void func_00150720(struct S150720 *object, unsigned char index, float value)
{
    object->values[index] = value;
}
