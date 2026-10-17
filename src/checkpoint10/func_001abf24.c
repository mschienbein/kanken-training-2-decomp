struct W23FlowScreen {
    unsigned char reserved00[0x90];
    int stack_index;
    int modes[2];
    int previous_modes[2];
};
extern void func_001737c0(struct W23FlowScreen *screen, int enabled);
extern void func_00181b68(unsigned mask, int direction, int animate);
extern int func_00177124(unsigned mask);

static void set_mode(struct W23FlowScreen *screen, int mode)
{
    screen->modes[screen->stack_index] = mode;
}

static void change_mode(struct W23FlowScreen *screen, int mode)
{
    screen->previous_modes[screen->stack_index] = screen->modes[screen->stack_index];
    set_mode(screen, mode);
}

void func_001abf24(struct W23FlowScreen *screen)
{
    switch (screen->modes[screen->stack_index]) {
    case 21:
        func_00181b68(7, 1, 1);
        change_mode(screen, 22);
        break;
    case 22:
        if (func_00177124(7) == 0) {
            func_001737c0(screen, 0);
            func_00181b68(7, 0, 1);
            change_mode(screen, 23);
        }
        break;
    case 23: {
        int state = func_00177124(7);
        if (state == 0)
            screen->stack_index = state;
        break;
    }
    }
}
