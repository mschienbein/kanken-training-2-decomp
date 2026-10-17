void func_0014b2e8(unsigned *destination, const unsigned *source,
                   unsigned stride, const unsigned *offsets)
{
    int i;
    /* Entry zero is not read; entries four through one are used. */
    for (i = 4; i > 0; --i) {
        const unsigned *row = source + offsets[i];
        *destination++ = row[0];
        *destination++ = row[stride];
        *destination++ = row[1];
        *destination++ = row[stride + 1];
        row += 2 * stride;
        *destination++ = row[0];
        *destination++ = row[stride];
        *destination++ = row[1];
        *destination++ = row[stride + 1];
    }
}
