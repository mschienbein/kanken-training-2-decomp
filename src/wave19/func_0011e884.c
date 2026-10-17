extern void func_001228f0(unsigned int display, unsigned int buffer,
    const void *left, const void *right, unsigned int stride,
    unsigned int format, unsigned int selection);

void func_0011e884(unsigned int display, unsigned int buffer,
    const void *left, const void *right, unsigned int stride,
    unsigned int format, unsigned int selection)
{
    func_001228f0(display, buffer, left, right, stride, format, selection);
}
