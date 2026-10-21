#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "game/menu_scene.h"

struct MenuControlSet;
#ifdef __cplusplus
class ScreenController;
#endif

#include "game/w11_control.h"
#include "game/screen.h"
#include "game/functions.h"

#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */

class ScreenController : public MenuScene {
public:
    virtual void build();
    virtual ~ScreenController();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_00162f78 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int selected, previous;
    int substates[8];
    int timer;
    unsigned char reservedbc[0x48];
    int result;
    unsigned char reserved108[0x280];
    void *resource;
};

extern "C" {

void func_0017457c(MenuControlSet *screen);
void func_0015fc3c(ScreenController *controller);
void func_00160694(ScreenController *scene);
void func_00161240(ScreenController *scene);
void func_001608b4(ScreenController *scene);
int func_0017a210(const AnimationState *state);
void func_0017a164(Screen *screen, int state);
}
#endif

#endif
