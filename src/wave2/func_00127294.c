struct RegisterState {
    unsigned char reserved00[0x770];
    unsigned control;
};

extern struct RegisterState *data_00242fd0;
extern void func_00132248(void);

void func_00127294(unsigned *state)
{
    struct RegisterState *registers = data_00242fd0;
    *state = 0x1c00;
    registers->control = (registers->control & ~0x2000u) | 0x11000;
    func_00132248();
}
