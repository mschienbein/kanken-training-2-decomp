const unsigned short *func_0015ddec(const unsigned short *text,
                                  const unsigned short *needle)
{
    if (*needle == 0)
        return text;
    while (*text != 0) {
        const unsigned short *candidate = text;
        const unsigned short *part = needle;
        while (*candidate != 0 && *part != 0 && *candidate == *part) {
            ++candidate;
            ++part;
        }
        if (*part == 0)
            return text;
        ++text;
    }
    return 0;
}
