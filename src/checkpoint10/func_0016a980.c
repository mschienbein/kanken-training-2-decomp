/* [INFERENCE] Role unrecovered: 78 bytes per element plus 57, with the low two bits cleared. */
unsigned int func_0016a980(unsigned int capacity)
{
    unsigned int size = capacity * 78u;
    return (size + 57u) & ~3u;
}
