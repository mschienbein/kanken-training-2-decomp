extern unsigned short data_0024389e;
extern unsigned short bss_004e051c[];

unsigned char func_001599ac(unsigned short character)
{
    unsigned short index;
    unsigned short count = data_0024389e;
    unsigned char kana_index = 0;
    for (index = 0; index < count; ++index) {
        unsigned short code = bss_004e051c[index];
        if ((unsigned int)(code - 0x30) <= 9 ||
            (unsigned int)(code - 0x41) <= 0x19 ||
            (unsigned int)(code - 0x61) <= 0x19 ||
            (unsigned int)(code - 0x3041) <= 0x5e ||
            (unsigned int)(code - 0x30a1) <= 0x5e) {
            if (code == character)
                return kana_index;
            ++kana_index;
        } else if (index < 0x100 && code == character) {
            /* ORIGINAL BUG: indices 246 through 255 wrap the byte-sized result. */
            return index + 10;
        }
    }
    return 0xff;
}
