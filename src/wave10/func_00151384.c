struct S151384 {
    unsigned char reserved[0xd0];
    short values[];
};

void func_00151384(struct S151384 *object, int index, short value)
{
    object->values[index] = value;
}
