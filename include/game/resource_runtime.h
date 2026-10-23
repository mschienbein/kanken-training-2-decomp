#ifndef GAME_RESOURCE_RUNTIME_H
#define GAME_RESOURCE_RUNTIME_H
#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Resource-event names; array stride and active/owned-pointer fields
 * are established by cleanup and registry consumers, not zero stores alone. */
struct ResourceNode;
struct ResourceEventPool {
    unsigned reserved00[3];
    struct DisplayObject *events;
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

struct AssetSlot { struct DisplayObject *asset; };
struct W17SceneVTable;
struct DisplayResource4c {
    struct DisplayObject *object;
    unsigned char reserved04[0x48];
};
struct DisplayResource50 {
    struct DisplayObject *object;
    unsigned char reserved04[0x2c];
    void *allocation;
    unsigned char reserved34[0x1c];
};
struct DisplayResource6c {
    struct DisplayObject *object;
    unsigned char reserved04[0x64];
    int texture_slot;
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
    void *preliminary[2];
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
struct GeometryAssetSlot;
struct ReviewScreenState {
    struct ScreenResourceManager resources;
    unsigned char reserved90[0x14];
    int index;
    unsigned char reserveda8[0x10];
    int option;
    unsigned char reservedbc[0x38];
    struct DisplayObject *first_asset;
    unsigned char reservedf8[0xc0];
    struct DisplayObject *second_asset;
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
extern void func_0019d3b4(struct DisplayObject *event);
extern void func_00183208(struct DisplayObject *event);
extern void func_0018663c(struct DisplayObject *event);
extern void func_00176e24(struct ScreenResourceManager *manager);
extern void func_001e3548(struct W17SelectionOwner *component);
#ifdef __cplusplus
}
#endif

#endif
