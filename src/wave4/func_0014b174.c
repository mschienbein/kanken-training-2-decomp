struct ByteFlags {
    unsigned char reserved[3];
    unsigned char value;
};

unsigned char func_0014b174(const struct ByteFlags *flags)
{
    return flags->value;
}
