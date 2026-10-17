struct AnimationState {
    unsigned char reserved00[0x44];
    float start;
    float finish;
    unsigned char reserved4c[0x0c];
    float lower;
    float upper;
    float current;
};

struct W11ControlScreen;

/* INTEGRATOR: move to include/game/w11_control.h */
extern "C" {
int func_0017a210(const AnimationState *state);
void func_001b0600(W11ControlScreen *screen);
void func_0016d554(W11ControlScreen *screen, int state);
}

struct W11AnimatedScreenA {
    virtual void reserved_virtual00() = 0;
    virtual void reserved_virtual04() = 0;
    virtual void reserved_virtual08() = 0;
    virtual void reserved_virtual0c() = 0;
    virtual void reserved_virtual10() = 0;
    virtual void reserved_virtual14() = 0;
    virtual void begin_frame() = 0;
    virtual void reserved_virtual1c() = 0;
    virtual void end_frame() = 0;
    unsigned char reserved04[0x68];
    const AnimationState *animation;
    unsigned char reserved70[0x404];
    W11ControlScreen *controls;
};

extern "C" void func_001b9ebc(W11AnimatedScreenA *screen)
{
    screen->begin_frame();
    func_001b0600(screen->controls);
    if (screen->controls != 0 && screen->animation != 0)
        func_0016d554(screen->controls, func_0017a210(screen->animation));
    screen->end_frame();
}
