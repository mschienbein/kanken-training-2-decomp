#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

struct MotionScreen;
#ifdef __cplusplus
class ScreenController;
#endif

#include "game/w11_control.h"
#include "game/screen.h"
#include "game/functions.h"

#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */

class ScreenController {
public:
    virtual ~ScreenController();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void update();
    virtual void slot07();
    virtual void finish();
    unsigned char reserved04[0x48];
    MotionScreen *motions;
    unsigned char reserved50[0x1c];
    AnimationState *animation;
    unsigned char reserved70[0x14];
    Screen *indicators;
    unsigned char reserved88[0x300];
    void *resource;
};

extern "C" {

void func_0017457c(MotionScreen *screen);
void func_0015fc3c(ScreenController *controller);
int func_0017a210(const AnimationState *state);
void func_0017a164(Screen *screen, int state);
}
#endif

#endif
