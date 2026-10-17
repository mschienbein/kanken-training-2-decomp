unsigned short func_0017d644(unsigned char x1, unsigned char y1,
                            unsigned char x2, unsigned char y2)
{
    int dx = x2 - x1;
    int dy;
    unsigned short width, height;
    if (dx <= 0)
        dx = x1 - x2;
    width = (unsigned short)dx;
    dy = y2 - y1;
    if (dy <= 0)
        dy = y1 - y2;
    height = (unsigned short)dy;
    return (width >= height ? width + (height >> 1) : height + (width >> 1)) + 1;
}
