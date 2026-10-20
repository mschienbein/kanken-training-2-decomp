/* The caller searches a UTF-16 string for a space and advances past the result. */
const unsigned short *func_0015dd5c(const unsigned short *text, unsigned value)
{
    for (;;) {
        unsigned current = *text;
        if (current == value)
            return text;
        if (current == 0)
            return 0;
        ++text;
    }
}
