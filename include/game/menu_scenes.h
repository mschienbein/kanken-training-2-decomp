#ifndef GAME_MENU_SCENES_H
#define GAME_MENU_SCENES_H
#include "game/menu_controller.h"
#include "game/category_progress.h"
#include "game/training_screen.h"
#include "game/drawing_screen.h"
#include "game/controller.h"
#include "game/playback_screen.h"
#include "game/controller_transitions.h"
#include "game/effects_menu.h"
#include "game/resource_runtime.h"

#ifdef __cplusplus
struct W17SmallCollection;
struct W6DisplayCollection;
struct W17WideCollection;
struct CalendarScene : MenuScene {
    virtual void build();
    virtual ~CalendarScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001b325c; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001b325c constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    unsigned char reserved90[0x190];
    W17SmallCollection *small;
    W6DisplayCollection *display;
    W17WideCollection *wide;
};
/* [INFERENCE] Descriptive identity, fixed by its original constructor
 * and singleton; only the common initialized scene prefix is completed. */
struct SceneAt0028db2c : MenuScene {
    virtual void build();
    virtual ~SceneAt0028db2c();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001bb8c0; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001bb8c0 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    unsigned char reserved90[0x14];
    int selection_value;
    unsigned char reserveda8[0x18c];
    AssetSlot **slots3, **slots5;
    GeometryAssetSlot **geometry;
};
/* [INFERENCE] Descriptive identity, fixed by its original constructor
 * and singleton; only the common initialized scene prefix is completed. */
struct SceneAt0028dd7c : MenuScene {
    virtual void build();
    virtual ~SceneAt0028dd7c();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001c0d30; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001c0d30 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    unsigned char reserved90[0x28];
    int option;
    unsigned char reservedbc[0x38];
    AssetSlot first_asset;
    unsigned char reservedf8[0xc0];
    AssetSlot second_asset;
    unsigned char reserved1bc[0x20];
    MotionState motion;
    AssetSlot **slots19, **slots18;
};
/* [INFERENCE] Descriptive identity, fixed by its original constructor
 * and singleton; only the common initialized scene prefix is completed. */
struct SceneAt0028e27c : MenuScene {
    virtual void build();
    virtual ~SceneAt0028e27c();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001b7294; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001b7294 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
};
#endif
#endif
