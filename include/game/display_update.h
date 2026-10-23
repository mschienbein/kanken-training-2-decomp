#ifndef GAME_DISPLAY_UPDATE_H
#define GAME_DISPLAY_UPDATE_H
#include "game/functions.h"
#include "game/screen.h"
#include "game/selection_components.h"

struct Component001b4310;
struct Component001ad7a0;
struct Component001b6098;
struct Component001b4d58;
struct TransitionController;
struct DisplayUpdateEntry4c {
    struct DisplayObject *object;
    unsigned char reserved04[0x48];
};
struct DisplayUpdateEntry50 {
    struct DisplayObject *object;
    unsigned char reserved04[0x4c];
};
/* The third update vector has the exact 0x6c stride and fields consumed by
 * func_001ebca0; its first argument is retained as opaque update context. */
struct DisplayUpdateManager {
    unsigned reserved00[2];
    struct DisplayUpdateEntry4c *entries4c;
    struct DisplayUpdateEntry50 *entries50;
    struct PendingDisplayUpdate *entries6c;
    unsigned reserved14[3];
    int count4c, count50, count6c;
    unsigned reserved2c[3];
    struct DeferredResourceList20 *component38;
    struct DeferredResourceList16 *component3c;
    struct DeferredRequest *component40;
    struct Component001b3564 *component44;
    struct Component001eaaac *component48;
    struct Component001e9e7c *component4c;
    struct Component001eaf70 *component50;
    struct Component0015eb90 *component54;
    void *component58;
    struct Component001ad134 *component5c;
    struct Component001acc1c *component60;
    struct Component001b6b88 *component64;
    struct Component001b4718 *component68;
    struct Component001b4310 *component6c;
    struct Component001af244 *component70;
    struct Component001acebc *component74;
    struct Component001ad7a0 *component78;
    struct Component001b6098 *component7c;
    struct Component001b4d58 *component80;
    struct Component001b7c88 *component84;
    struct W19SelectionPanel *component88;
    struct TransitionController *component8c;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001b4310(struct Component001b4310 *component);
extern void func_001ad7a0(struct Component001ad7a0 *component);
extern void func_001b6098(struct Component001b6098 *component);
extern void func_001b4d58(struct Component001b4d58 *component);
extern int func_001b6604(struct TransitionController *group);
extern void func_001ebb14(struct DisplayUpdateManager *manager, struct DisplayUpdateEntry4c *entry);
extern void func_001eb95c(struct DisplayUpdateManager *manager, struct DisplayUpdateEntry50 *entry);
#ifdef __cplusplus
}
#endif
#endif
