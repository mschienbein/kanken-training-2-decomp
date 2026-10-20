#ifndef GAME_SCENE_LIFECYCLE_H
#define GAME_SCENE_LIFECYCLE_H

struct AudioChannel;
struct PoolLink;
struct PoolList;
struct SceneAggregate;
struct SceneBase;
struct SceneController;
struct SceneGroup;
struct SoundPlaybackState;
struct ObjectPool;
struct W17CompositeScene;
struct W17ExtendedScene;
struct W18SceneA;
struct W17SceneVTable;
struct W18SceneB;
struct W18SceneC;
struct SceneToggle;
struct W17SmallCollection;
struct W6DisplayCollection;
struct W17WideCollection;
struct W18DisplayCollection;
struct W18WideDisplayCollection;
struct DispatchStateA;
struct DispatchStateB;
struct ModeDispatchObject;
struct DisplayUpdateManager;
struct W6SelectionScreen;

#include "game/notification.h"
#include "game/scene_dispatch.h"
#include "game/selection.h"
#include "game/resource_runtime.h"










#include "game/pool_list.h"
#include "game/audio_voice.h"
#include "game/sound_track.h"
#include "game/sound_runtime.h"

#include <stdbool.h>
#include "game/functions.h"



struct SceneEngine {
    const void *vtable;
    unsigned char reserved04[0x4d];
    unsigned char inhibited;
    unsigned char reserved52[0x32];
    struct PoolList active;
#ifdef __cplusplus
    ~SceneEngine();
#endif
};



#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
extern nw::snd::internal::driver::Voice *func_00138cb0(nw::snd::internal::driver::Voice *resource);
#endif
#ifdef __cplusplus
extern void func_00152f10(nw::snd::internal::driver::Voice *resource, int event, void *context);
#endif
#ifdef __cplusplus
extern void func_0018290c(nw::snd::internal::driver::Voice *resource, int event, void *context);
#endif

extern struct ObjectPool *func_00185cc0(void);
extern void func_00182994(struct ObjectPool *pool, void *object);
extern void func_001829d0(struct AudioChannel *channel);
#ifdef __cplusplus
extern void func_00183550(nw::snd::internal::driver::Voice *group);
#endif
#ifdef __cplusplus
extern void func_0016a27c(nw::snd::internal::driver::Voice *resource, int enabled);
#endif
extern void func_001d4cf4(struct SoundTrack *parent, bool enabled);
#ifdef __cplusplus
}
#endif


struct SceneGroup {
    unsigned char reserved00[0xe];
    unsigned char enabled;
    unsigned char reserved0f[0x90 - 0xf];
    struct SoundTrack *children[16];
};

#ifdef __cplusplus
struct SceneBase : SoundPlaybackState {
    ~SceneBase()
    {
        vtable = data_0022459c;
        active = 3;
    }
};
#endif

#ifdef __cplusplus
struct SceneController {
    const void *vtable;
    /* Member extent follows from the engine at aggregate offset 0x2314;
     * no internal controller-record capacity is assumed. */
    unsigned char reserved04[0x21fc - 4];
    ~SceneController();
};
#endif

#ifdef __cplusplus
struct SceneAggregate : SceneBase {
    SceneController controller;
    SceneEngine engine;
    ~SceneAggregate();
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_001d8960(struct SceneGroup *group, bool enabled);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */










struct W17CompositeScene {
    ScreenResourceManager base;
    unsigned char reserved90[0x190];
    W17SmallCollection *small;
    W6DisplayCollection *display;
    W17WideCollection *wide;
};
struct W17ExtendedScene {
    ScreenResourceManager base;
    int selection;
};
struct W18SceneA {
    ScreenResourceManager base;
    unsigned char reserved90[0x24];
    W18DisplayCollection *objects;
};
/* The noninvoked prefix permits lifecycle installers to retain the actual
 * table address without claiming a common callable receiver. Concrete table
 * tails below state the real providers' types. No call through this prefix
 * is valid: the corresponding complete scene-owner relation is unrecovered. */
struct W17SceneVTable {
    unsigned char reserved00[0x18];
};
struct W17InactiveSceneVTable : W17SceneVTable {
    unsigned reserved18;
    unsigned reserved1c;
    void (*finish)(DisplayUpdateManager *scene);
};
struct W18SceneAVTable : W17SceneVTable {
    void (*prepare)(DispatchStateA *scene);
    unsigned reserved1c;
    void (*finish)(DisplayUpdateManager *scene);
};
struct W18SceneBVTable : W17SceneVTable {
    void (*prepare)(DispatchStateB *scene);
    unsigned reserved1c;
    void (*finish)(DisplayUpdateManager *scene);
};
struct W18SceneCVTable : W17SceneVTable {
    void (*prepare)(ModeDispatchObject *scene);
    unsigned reserved1c;
    void (*finish)(DisplayUpdateManager *scene);
};
struct W17SelectionSceneVTable : W17SceneVTable {
    void (*prepare)(W6SelectionScreen *scene);
    unsigned reserved1c;
    void (*finish)(DisplayUpdateManager *scene);
};
struct W18SceneB {
    ScreenResourceManager base;
    unsigned char reserved90[0x1c];
    W18DisplayCollection *objects;
};
struct W18SceneC {
    ScreenResourceManager base;
    unsigned char reserved90[0xec];
    W18WideDisplayCollection *objects;
};
struct SceneToggle {
    unsigned char reserved00[14];
    bool enabled;
    unsigned char reserved0f[0x8d];
    struct SoundTrack *child;
};

extern "C" {





int func_00177124(int mode);
void func_00177084(void);
void func_001c1abc(W17SmallCollection *collection);
void func_001c29ac(W6DisplayCollection *collection);
void func_001c2d74(W17WideCollection *collection);
int func_0016f9c0(unsigned int mask);
void func_001dc240(unsigned int mask);
void func_00170f20(int index);
TransformEntry *func_00174454(int index, int part);
void func_00176df4(void);
extern const W17InactiveSceneVTable data_0022360c;
void func_00174480(void *object);
extern const W17InactiveSceneVTable data_0022380c;
extern const W17SelectionSceneVTable data_00223838;
extern const W17InactiveSceneVTable data_00223864;
extern const W17InactiveSceneVTable data_00223908;

int func_00171e30(void);
void func_00170f6c(W18DisplayCollection *collection);
int func_001c20e8(W18DisplayCollection *collection);
extern const W18SceneAVTable data_002235c8;
extern const W18SceneBVTable data_00223720;

void func_001c13f4(W18WideDisplayCollection *collection);
extern const W18SceneCVTable data_00223934;





















void func_001d5c50(struct SceneToggle *toggle, bool enabled);
}
#endif

#endif
