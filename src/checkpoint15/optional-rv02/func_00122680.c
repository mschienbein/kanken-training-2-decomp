/* Bounded small-divisor screening with unsigned candidate wrapping. */
unsigned func_00122680(unsigned value)
{
    unsigned offset;
    if (value <= 3)
        return 3;
    if (value <= 19)
        return value | 1;
    for (offset = 0; offset < 100; ++offset) {
        unsigned candidate = value + offset;
        if (!(candidate & 1)) continue;
        if (candidate % 3 == 0) continue;
        if (candidate % 5 == 0) continue;
        if (candidate % 7 == 0) continue;
        if (candidate % 11 == 0) continue;
        if (candidate % 13 == 0) continue;
        if (candidate % 17 == 0) continue;
        return candidate;
    }
    return value | 1;
}
