struct S151614 {
    unsigned char reserved0[0x0c];
    unsigned char active;
    unsigned char reserved1[0x6c - 0x0d];
    int integer_value;
    float float_value;
};

void func_00151614(struct S151614 *object, unsigned char mode, int amount)
{
    if (object->active) {
        if (mode == 0)
            object->integer_value += amount;
        else if (mode == 1)
            object->float_value += amount;
    }
}
