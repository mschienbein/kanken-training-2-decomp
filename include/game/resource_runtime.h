#ifndef GAME_RESOURCE_RUNTIME_H
#define GAME_RESOURCE_RUNTIME_H
#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Resource-event names; array stride and active/owned-pointer fields
 * are established by cleanup and registry consumers, not zero stores alone. */
struct ResourceNode;
struct ResourceEventPool {
    unsigned reserved00[3];
    struct ResourceEvent *events;
    struct ResourceNode *first;
    struct ResourceNode *second;
    struct ResourceNode *third;
    int count;
    int active_count;
};
struct ResourceControlState {
    unsigned ready;
    unsigned state04;
    unsigned state08;
    unsigned char state0c;
    unsigned char reserved0d[3];
    int value;
};

struct AssetSlot { struct ResourceEvent *asset; };
struct ScreenResourceManager;
/* [INFERENCE] Shared writable scene dispatch and resource-cleanup protocol.
 * The initializer/global linkage proves this common prefix, not each complete
 * static object's extent or historical native class. */
struct W17SceneVTable {
    unsigned char reserved00[8];
    void (*slot02)(struct ScreenResourceManager *scene);
    unsigned char reserved0c[8];
    void (*slot05)(struct ScreenResourceManager *scene);
    void (*prepare)(struct ScreenResourceManager *scene);
    unsigned reserved1c;
    void (*finish)(struct ScreenResourceManager *scene);
};
struct DisplayResource4c {
    struct ResourceEvent *object;
    unsigned char reserved04[0x48];
};
struct DisplayResource50 {
    struct ResourceEvent *object;
    unsigned char reserved04[0x2c];
    void *allocation;
    unsigned char reserved34[0x1c];
};
struct DisplayResource6c {
    struct ResourceEvent *object;
    unsigned char reserved04[0x64];
    int texture_slot;
};
struct W17SecondaryState {
    unsigned reserved00[2];
    unsigned value;
    unsigned active;
};
struct W17PrimaryState {
    unsigned reserved00[3];
    unsigned value;
    unsigned active;
};
struct ScreenResourceManager {
    const struct W17SceneVTable *vtable;
    unsigned reserved04;
    struct DisplayResource4c *first;
    struct DisplayResource50 *second;
    struct DisplayResource6c *third;
    struct W10DrawingInput *controller;
    unsigned char reserved18[8];
    int first_count, second_count, third_count;
    unsigned char reserved2c[8];
    int index;
    struct W17SecondaryState *secondary;
    struct W17PrimaryState *primary;
    struct W17SelectionOwner *component00;
    struct Component001b3434 *component01;
    struct Component001ea87c *component02;
    struct Component001e9cdc *component03;
    struct Component001eaed8 *component04;
    struct Component00168454 *component05;
    struct Component001af424 *component06;
    struct Component001acff0 *component07;
    struct Component001682e8 *component08;
    struct Component001b6a0c *component09;
    struct Component001b45ac *component10;
    struct Component001b41b8 *component11;
    struct Component001aeb30 *component12;
    struct Component001acd38 *component13;
    struct Component001ad708 *component14;
    struct Component001b5e44 *component15;
    struct Component001b494c *component16;
    struct Component001b7604 *component17;
    struct Component001ae140 *component18;
    struct Component001b64b0 *component19;
};
/* [INFERENCE] Only the naturally used extension after the cleanup prefix is
 * named; constructor/complete historical class identity remains unrecovered. */
struct TrainingMenuScene {
    struct ScreenResourceManager base;
    unsigned char reserved90[0x20];
    int selection;
    unsigned char reservedb4[0x20];
    int overlay_active;
    unsigned char reservedd8[0x2b0];
    void *transient;
};
struct MenuRowManager;
struct PagedMenuSceneA {
    struct ScreenResourceManager base;
    unsigned char reserved90[0x3e4];
    struct MenuRowManager *rows;
};
struct PagedMenuSceneB {
    struct ScreenResourceManager base;
    unsigned char reserved90[0x440];
    struct MenuRowManager *rows;
};
/* Both natural cleanup entries pass this receiver to resource cleanup and
 * preserve the same base index; no independent menu-prefix identity is used. */
struct W24BMenuState {
    struct ScreenResourceManager base;
    unsigned char reserved90[0x50];
    struct WidgetSlot **selection_slots;
    struct WidgetSlot **footer_slots;
};
struct EffectsMenuScene {
    struct ScreenResourceManager base;
    unsigned char reserved90[0xf0];
    struct ResourceEvent *display;
    unsigned char reserved184[0x20];
    struct MotionState motion;
};
/* [INFERENCE] Cleanup-only extensions of the same resource-manager protocol.
 * Complete native constructor/lifecycle identities remain unrecovered. */
struct GridCleanupScreen {
    struct ScreenResourceManager base;
    unsigned char reserved90[0x14];
    struct DisplayHandle **six_handles;
    struct DisplayHandle **four_handles;
};
struct MotionCleanupScreen {
    struct ScreenResourceManager base;
    unsigned char reserved90[0x10];
    struct DisplayHandle **six_handles;
    struct DisplayHandle **four_handles;
    struct MotionState motion;
};
struct GeometryAssetSlot;
struct ReviewScreenState {
    struct ScreenResourceManager resources;
    unsigned char reserved90[0x14];
    int index;
    unsigned char reserveda8[0x10];
    int option;
    unsigned char reservedbc[0x38];
    struct ResourceEvent *first_asset;
    unsigned char reservedf8[0xc0];
    struct ResourceEvent *second_asset;
    unsigned char reserved1bc[0x20];
    struct MotionState motion;
    struct AssetSlot **slots19;
    struct AssetSlot **slots18;
    unsigned char reserved218[0x1c];
    struct AssetSlot **slots3;
    struct AssetSlot **slots5;
    struct GeometryAssetSlot **geometry;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceEventPool data_00242238;
extern struct ResourceControlState bss_00495c34, bss_00495c48, bss_00495c5c;
extern void func_0019d3b4(struct ResourceEvent *event);
extern void func_00183208(struct ResourceEvent *event);
extern void func_0018663c(struct ResourceEvent *event);
extern void func_00176e24(struct ScreenResourceManager *manager);
extern void func_001e3548(struct W17SelectionOwner *component);
extern void func_00179a48(struct TrainingMenuScene *scene, int active);
extern void func_00161650(struct TrainingMenuScene *scene);
extern void func_001a7444(struct ScreenResourceManager *scene);
extern void func_001ab540(struct ScreenResourceManager *scene);
extern void func_001b6db4(struct ScreenResourceManager *scene);
extern void func_001b72b8(struct ScreenResourceManager *scene);
extern void func_00168454(struct Component00168454 *component);
extern void func_001682e8(struct Component001682e8 *component);
extern void func_001e381c(struct GridCleanupScreen *screen);
extern void func_001e3ec8(struct GridCleanupScreen *screen);
extern void func_001e8024(struct MotionCleanupScreen *screen);
extern void func_001e8738(struct MotionCleanupScreen *screen);
extern const struct W17SceneVTable data_002237b4, data_00223890;
extern int func_0017731c(struct W17SecondaryState *state);
extern int func_00177160(struct W17PrimaryState *state);
/* [INFERENCE] Unlocated opaque endpoints: their receivers follow retained
 * natural cleanup callers; complete owners and result contracts stay qualified. */
extern void func_0016d394(struct MenuRowManager *rows);
extern void func_001c3c50(struct GeometryAssetSlot **slots);
extern void func_001b8ba0(struct PagedMenuSceneA *scene);
extern void func_001ba1f8(struct PagedMenuSceneA *scene);
extern void func_001bc538(struct PagedMenuSceneB *scene);
extern void func_001bd968(struct PagedMenuSceneB *scene);
extern void func_001bb8e4(struct ReviewScreenState *scene);
extern void func_001c0d54(struct ReviewScreenState *scene);
extern void func_001e440c(struct EffectsMenuScene *scene);
extern void func_001a2a18(struct W24BMenuState *scene);
extern void func_001a0200(struct W24BMenuState *scene);
extern void func_001c0f5c(struct WidgetSlot **slots);
extern void func_001c1cfc(struct WidgetSlot **slots);
extern void func_001e567c(struct EffectsMenuScene *scene);
extern const struct W17SceneVTable data_0022368c, data_002236b8;
extern const struct W17SceneVTable data_002236f4, data_0022374c, data_002237e0;
/* [INFERENCE] Constructor entry spelling and complete native class identities.
 * The full-word signed counts and unchanged normal-return receiver are backed
 * by the natural constructor, signed cleanup consumers and bounded AAPCS proof. */
extern struct ScreenResourceManager *func_00174484(struct ScreenResourceManager *object,
    int first_count, int second_count, int third_count);
extern struct ScreenResourceManager *func_00162f78(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001a29f4(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001a7420(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001ab51c(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001acb90(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001b325c(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001b7294(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001ba1d4(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001bb8c0(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001bd944(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001be104(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001c0d30(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e3270(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e3ea4(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e5658(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e744c(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e7c4c(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e7fb8(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001e8714(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001ec128(struct ScreenResourceManager *object);
extern struct ScreenResourceManager *func_001ef790(struct ScreenResourceManager *object);
extern const struct W17SceneVTable data_00223518, data_00223544, data_00223788;
#ifdef __cplusplus
}
#endif

#endif
