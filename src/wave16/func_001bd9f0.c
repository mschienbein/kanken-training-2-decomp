struct DispatchStateB {
    unsigned char reserved00[0x90];
    int mode;
};

extern void func_001bdb70(struct DispatchStateB *state);
extern void func_001bda14(struct DispatchStateB *state);

void func_001bd9f0(struct DispatchStateB *state)
{
    switch (state->mode) {
    case 0:
        func_001bda14(state);
        break;
    case 1:
        func_001bdb70(state);
        break;
    }
}
