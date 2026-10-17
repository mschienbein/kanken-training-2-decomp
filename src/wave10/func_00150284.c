struct S150284 {
    unsigned char reserved[0x10c];
    short values[];
};

void func_00150284(struct S150284 *object, int index, short value)
{
    object->values[index] = value;
}
