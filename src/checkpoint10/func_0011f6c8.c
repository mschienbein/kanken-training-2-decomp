/* [INFERENCE] The final three words are an aligned 64-bit identifier and a word. */
extern int func_0012d680(unsigned *out, void *records, unsigned count,
                         unsigned long long identifier, unsigned flags);

int func_0011f6c8(unsigned *out, void *records, unsigned count,
                  unsigned long long identifier, unsigned flags)
{
    return func_0012d680(out, records, count, identifier, flags);
}
