extern void func_00111114(void *destination, const void *source,
                           unsigned size, int release_source);

void func_001110f0(unsigned char *destination, unsigned offset,
                   const void *source, unsigned size)
{
    func_00111114(destination + offset, source, size, 1);
}
