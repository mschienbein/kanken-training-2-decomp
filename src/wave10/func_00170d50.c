unsigned short *func_00170d50(unsigned short *destination, unsigned size,
                             const unsigned short *source)
{
    unsigned short *cursor = destination;
    int length = 0;
    int remaining;
    unsigned short character;
next_character:
    if (*cursor != 0) {
        ++cursor;
        ++length;
        goto next_character;
    }
    cursor = destination + length;
    remaining = size >> 1;
    /* ORIGINAL BUG: size is not reduced by the existing string's length, and
     * appending does not write a new terminator. */
    while ((character = *source) != 0 && remaining > 0) {
        --remaining;
        ++source;
        *cursor++ = character;
    }
    return destination;
}
