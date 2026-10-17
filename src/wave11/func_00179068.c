/* Shared layout and interface from src/wave7/func_0015fae4.c. */
struct W7dMenuControl {
    unsigned char reserved00[0x20];
    unsigned *flags[];
};

void func_00179068(struct W7dMenuControl *control, int index, int enabled)
{
    unsigned *flags = control->flags[index];
    if (flags != 0) {
        if (enabled)
            *flags |= 4;
        else
            *flags &= ~4u;
    }
}
