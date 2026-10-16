struct DisplayObject {
    unsigned char reserved00[0x92];
    unsigned char types[2];
};

unsigned char func_0017a9a0(const struct DisplayObject *object, int index)
{
    return object->types[index];
}
