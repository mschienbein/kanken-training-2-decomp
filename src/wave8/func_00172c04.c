struct TransitionController {
    unsigned char reserved00[0x34];
    unsigned char active;
    unsigned char paused;
    unsigned char reserved36[2];
    float elapsed;
    unsigned char reserved3c[4];
    int mode;
};

extern void func_001b6788(struct TransitionController *controller, int mode);

void func_00172c04(struct TransitionController *controller, int mode)
{
    controller->paused = 0;
    controller->mode = mode;
    controller->active = 1;
    controller->elapsed = 0.0f;
    func_001b6788(controller, mode);
}
