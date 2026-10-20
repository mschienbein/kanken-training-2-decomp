#ifndef GAME_DEFERRED_CALLBACK_H
#define GAME_DEFERRED_CALLBACK_H
#include "game/menu_scene.h"

#ifdef __cplusplus
/* Native member descriptors belong to the constructor-proven scene hierarchy. */
typedef void (MenuScene::*SceneMemberCallback)();

struct DeferredMemberCallback {
    MenuScene *object;
    SceneMemberCallback method;
};

extern "C" {
extern const SceneMemberCallback data_002188d8;
extern struct DeferredMemberCallback data_0023efb0;
extern void func_001b7dac(void);
extern void func_0015dec4(void (*callback)(void));
extern void func_00177b10(MenuScene *object, SceneMemberCallback method);
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */




extern "C" {
extern SceneMemberCallback data_0023efb4;
}
#endif

#endif
