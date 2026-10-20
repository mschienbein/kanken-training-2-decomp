#ifndef GAME_EFFECTS_MENU_H
#define GAME_EFFECTS_MENU_H
#include "game/menu_scene.h"
#include "game/runtime.h"
#include "game/screen.h"
#ifdef __cplusplus
/* 001e5658 constructs singleton 0028f200 through MenuScene and installs
 * table 002237e0; 001e42fc directly passes this same receiver to 001e403c. */
struct EffectsMenuScene : MenuScene {
    virtual void build();
    virtual ~EffectsMenuScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001e5658 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int depth, phase, phase98, previous_phase, statea0;
    float elapsed;
    int statea8, entry;
    int text_length, stateb4, stateb8, edit_phase;
    unsigned short text[25];
    unsigned char reservedf2[0x8e];
    DisplayObject *cursor;
    unsigned char reserved184[0x20];
    MotionState motion;
};
extern "C" {
void func_001e403c(EffectsMenuScene *scene);
void func_001e42fc(EffectsMenuScene *scene);
}
#endif
#endif
