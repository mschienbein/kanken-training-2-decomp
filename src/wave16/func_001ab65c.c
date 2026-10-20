struct DispatchStateA {
    unsigned char reserved00[0x90];
    int mode;
};

extern void func_001abf24(struct DispatchStateA *state);
extern void func_001ab680(struct DispatchStateA *state);

void func_001ab65c(struct DispatchStateA *state)
{
    switch (state->mode) {
    case 0:
        func_001ab680(state);
        break;
    case 1:
        func_001abf24(state);
        break;
    }
}
