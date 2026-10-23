extern void func_001375bc(const void *source, void *destination, unsigned size,
                          unsigned flags, int validate_source);

void func_00132310(const void *source, void *destination, unsigned size,
                   unsigned flags)
{
    func_001375bc(source, destination, size, flags, 1);
}
