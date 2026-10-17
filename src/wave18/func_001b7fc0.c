struct FirstModeController {
    unsigned char reserved00[0x90];
    int mode;
};
extern void func_001b8238(struct FirstModeController *controller);
extern void func_001b8908(struct FirstModeController *controller);
extern void func_001b8424(struct FirstModeController *controller);

void func_001b7fc0(struct FirstModeController *controller)
{
    switch (controller->mode) {
    case 0: func_001b8908(controller); break;
    case 1: func_001b8424(controller); break;
    case 2: func_001b8238(controller); break;
    }
}
