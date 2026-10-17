int func_0015e088(unsigned character)
{
    unsigned offset = character - 0x3005;
    if (offset == 0 || offset == 2 || offset == 0x36)
        return 1;
    if (character - 0x3400 < 0x6c00 || character - 0xf900 < 0x200)
        return 1;
    if (character - 0x20000 < 0x10000)
        return 1;
    return 0;
}
