/* INTEGRATOR: differs from src/wave4/func_0019d048.c and func_0019d058.c:
 * this state view includes the writable linear_size word at +8; it does not
 * change the unrelated two-word MemorySpan type. */
struct LinearMemoryState {
    unsigned base;
    unsigned size;
    unsigned linear_size;
};
extern struct LinearMemoryState data_00242d68;
extern int func_00195cd8(unsigned *address, unsigned requested_address,
                        unsigned physical_address, unsigned size,
                        unsigned operation, unsigned permissions);

int func_0010bd7c(unsigned size)
{
    unsigned address;
    unsigned current;
    int result;
    if (size & 0xfff)
        return (int)0xe0e01bf2;
    current = data_00242d68.linear_size;
    if (size > current) {
        unsigned start = current + 0x08000000;
        unsigned length = size - current;
        result = func_00195cd8(&address, start, 0, length, 3, 3);
        if ((int)((unsigned)result & 0x80000000) >= 0)
            data_00242d68.linear_size = size;
    } else {
        unsigned start = size + 0x08000000;
        unsigned length = current - size;
        result = func_00195cd8(&address, start, 0, length, 1, 0);
        if ((int)((unsigned)result & 0x80000000) >= 0)
            data_00242d68.linear_size = size;
    }
    return result;
}
