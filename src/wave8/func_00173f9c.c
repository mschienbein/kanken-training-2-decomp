struct TransitionController {
    unsigned char reserved00[0x34];
    unsigned char active;
    unsigned char paused;
    unsigned char reserved36[2];
    float elapsed;
    unsigned char reserved3c[4];
    int mode;
};

void func_00173f9c(struct TransitionController *controller, int paused)
{
    controller->paused = paused;
    if (paused != 0)
        controller->elapsed = 0.0f;
}
