int func_0015ddc8(const unsigned short *const *text)
{
    int length = 0;
next_character:
    if ((*text)[length] != 0) {
        ++length;
        goto next_character;
    }
    return length;
}
