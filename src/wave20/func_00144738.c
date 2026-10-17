void func_00144738(unsigned format, unsigned *converted, unsigned char *native)
{
    unsigned result;
    *native = 1;
    switch (format & 0xffff) {
    case 0x6752: result = 0x1908; break;
    case 0x6754: result = 0x1907; break;
    case 0x6756: result = 0x1906; break;
    case 0x6757: result = 0x1909; break;
    case 0x6758: result = 0x190a; break;
    case 0x6759: result = 0x6700; break;
    case 0x675a: result = 0x675a; break;
    case 0x675b: result = 0x675b; break;
    case 0x675c: result = 0x6040; break;
    case 0x675d: result = 0x6050; break;
    default:
        *converted = format;
        *native = 0;
        return;
    }
    *converted = result;
}
