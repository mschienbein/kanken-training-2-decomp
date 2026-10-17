unsigned short *func_00178f28(unsigned short *destination, unsigned byte_limit,
                             const unsigned short *source)
{
    unsigned short *cursor = destination;
    unsigned remaining = byte_limit / 2;

    while (*source != 0 && remaining != 0) {
        --remaining;
        *cursor++ = *source++;
    }
    /* ORIGINAL BUG: a terminator is written even when the entire byte limit was copied. */
    *cursor = 0;
    return destination;
}
