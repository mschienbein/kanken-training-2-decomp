struct ValuePair151608 {
    int first;
    int second;
};

struct S151608 {
    unsigned char reserved[0x88];
    struct ValuePair151608 value;
};

void func_00151608(struct S151608 *object, struct ValuePair151608 value)
{
    object->value = value;
}
