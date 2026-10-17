#include "game/runtime.h"

struct ScreenAsset;
struct AssetSlot;
struct GeometryAssetSlot;
struct NotificationController;
/* [INFERENCE] Partial common screen layout, named by its two transition users. */
struct ReviewScreenState {
    unsigned char reserved00[0x14];
    struct NotificationController *controller;
    unsigned char reserved18[0x1c];
    int phase;
    unsigned char reserved38[0x6c];
    int index;
    unsigned char reserveda8[0x10];
    int option;
    unsigned char reservedbc[0x38];
    struct ScreenAsset *first_asset;
    unsigned char reservedf8[0xc0];
    struct ScreenAsset *second_asset;
    unsigned char reserved1bc[0x20];
    struct MotionState motion;
    struct AssetSlot **slots19;
    struct AssetSlot **slots18;
    unsigned char reserved218[0x1c];
    struct AssetSlot **slots3;
    struct AssetSlot **slots5;
    struct GeometryAssetSlot **geometry;
};

extern int func_00177124(unsigned mask);
extern void func_00177084(void);
extern void func_001c3854(struct AssetSlot **slots);
extern void func_001c42e8(struct AssetSlot **slots);
extern void func_00183208(struct ScreenAsset *asset);
extern void func_0016f024(struct MotionState *state);
extern void func_0016d4f8(struct NotificationController *controller);
extern void func_0016d310(struct NotificationController *controller);
extern void func_0016f55c(int group, int index, unsigned value, unsigned count,
                          unsigned flags, float x, float y);
extern void func_00176e24(struct ReviewScreenState *screen);
extern void func_00176df4(void);
extern void func_00176dc8(int phase);

void func_001be180(struct ReviewScreenState *screen)
{
    if (func_00177124(7))
        return;
    func_00177084();
    func_001c3854(screen->slots19);
    func_001c42e8(screen->slots18);
    if (screen->first_asset)
        func_00183208(screen->first_asset);
    if (screen->second_asset)
        func_00183208(screen->second_asset);
    func_0016f024(&screen->motion);
    if (screen->option) {
        func_0016d4f8(screen->controller);
        func_0016d310(screen->controller);
    }
    if (screen->option) {
        func_0016f55c(3, 1, 1, 0, 0, 0.0f, 0.0f);
        screen->phase = 10;
    } else {
        screen->phase = 4;
    }
    func_00176e24(screen);
    func_00176df4();
    func_00176dc8(screen->phase);
}
