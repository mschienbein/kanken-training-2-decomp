struct SecondModeController {
    unsigned char reserved00[0x90];
    int mode;
};
extern void func_001bbdd4(struct SecondModeController *controller);
extern void func_001bc2a0(struct SecondModeController *controller);
extern void func_001bbfc4(struct SecondModeController *controller);

void func_001bbc1c(struct SecondModeController *controller)
{
    switch (controller->mode) {
    case 0: func_001bc2a0(controller); break;
    case 1: func_001bbfc4(controller); break;
    case 2: func_001bbdd4(controller); break;
    }
}
