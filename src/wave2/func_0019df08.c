void *func_0019df08(void *destination, unsigned int size)
{
    unsigned char *cursor = destination;
    if (size == 0)
        return destination;
next_byte:
    --size;
    *cursor++ = 0;
    if (size != 0)
        goto next_byte;
    return destination;
}
