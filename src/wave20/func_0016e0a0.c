struct ResourceFormatContext;
/* [INFERENCE] The unused first input is the caller's format/context object. */
int func_0016e0a0(const struct ResourceFormatContext *context, unsigned format)
{
    switch (format) {
    case 0: case 1: case 2: case 3: case 4: return 0;
    case 5: return 1;
    case 6: return 2;
    case 7: case 9: return 3;
    case 8: return 4;
    case 10: return 5;
    case 11: case 12: case 13: case 14: case 15: case 16: return 6;
    case 17: return 7;
    case 18: return 8;
    case 19: return 9;
    case 20: case 21: case 22: return 10;
    case 23: return 11;
    case 24: return 12;
    case 25: return 13;
    case 26: return 14;
    case 27: case 28: case 29: return 15;
    case 30: return 16;
    case 32: return 32;
    default: return -1;
    }
}
