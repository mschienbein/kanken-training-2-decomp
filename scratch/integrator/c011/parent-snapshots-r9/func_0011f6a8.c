/* The IPC callee writes through both output pointers; the buffer descriptor scales count by the record size. */
extern int func_0012d550(unsigned *output, void *buffer, unsigned count,
                        unsigned char mode, unsigned first, unsigned second,
                        unsigned third);

int func_0011f6a8(unsigned *output, void *buffer, unsigned count,
                  unsigned char mode, unsigned first, unsigned second,
                  unsigned third)
{
    return func_0012d550(output, buffer, count, mode, first, second, third);
}
