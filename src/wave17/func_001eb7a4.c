#include "game/screen.h"

struct Component001dcf4c;
struct Component001dcdd0;
struct DeferredRequest;
struct Component001b3564;
struct Component001eaaac;
struct Component001e9e7c;
struct Component001eaf70;
struct Component0015eb90;
struct Component001ad134;
struct Component001acc1c;
struct Component001b6b88;
struct Component001b4718;
struct Component001b4310;
struct Component001af244;
struct Component001acebc;
struct Component001ad7a0;
struct Component001b6098;
struct Component001b4d58;
struct Component001b7c88;
struct Component001adff8;
struct TransitionController;

struct DisplayUpdateEntry4c {
    struct DisplayObject *object;
    unsigned char reserved04[0x48];
};
struct DisplayUpdateEntry50 {
    struct DisplayObject *object;
    unsigned char reserved04[0x4c];
};
struct DisplayUpdateEntry6c {
    struct DisplayObject *object;
    unsigned char reserved04[0x68];
};

struct DisplayUpdateManager {
    unsigned reserved00[2];
    struct DisplayUpdateEntry4c *entries4c;
    struct DisplayUpdateEntry50 *entries50;
    struct DisplayUpdateEntry6c *entries6c;
    unsigned reserved14[3];
    int count4c;
    int count50;
    int count6c;
    unsigned reserved2c[3];
    struct Component001dcf4c *component38;
    struct Component001dcdd0 *component3c;
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
    struct Component001adff8 *component88;
    struct TransitionController *component8c;
};

extern void func_001dcf4c(struct Component001dcf4c *component);
extern void func_001dcdd0(struct Component001dcdd0 *component);
extern void func_001e358c(struct DeferredRequest *request);
extern void func_001b3564(struct Component001b3564 *component);
extern int func_001eaaac(struct Component001eaaac *component);
extern void func_001e9e7c(struct Component001e9e7c *component);
extern int func_001eaf70(struct Component001eaf70 *component);
extern int func_0015eb90(struct Component0015eb90 *component);
extern void func_001acc18(void *object);
extern int func_001ad134(struct Component001ad134 *component);
extern int func_001acc1c(struct Component001acc1c *component);
extern int func_001b6b88(struct Component001b6b88 *component);
extern int func_001b4718(struct Component001b4718 *component);
extern void func_001b4310(struct Component001b4310 *component);
extern int func_001af244(struct Component001af244 *component);
extern int func_001acebc(struct Component001acebc *component);
extern void func_001ad7a0(struct Component001ad7a0 *component);
extern void func_001b6098(struct Component001b6098 *component);
extern void func_001b4d58(struct Component001b4d58 *component);
extern int func_001b7c88(struct Component001b7c88 *component);
extern void func_001adff8(struct Component001adff8 *component);
extern int func_001b6604(struct TransitionController *group);
extern void func_001ebb14(struct DisplayUpdateManager *manager, struct DisplayUpdateEntry4c *entry);
extern void func_001eb95c(struct DisplayUpdateManager *manager, struct DisplayUpdateEntry50 *entry);
extern void func_001ebca0(struct DisplayUpdateManager *manager, struct DisplayUpdateEntry6c *entry);

void func_001eb7a4(struct DisplayUpdateManager *manager)
{
    int index;
    if (manager->component38 != 0) func_001dcf4c(manager->component38);
    if (manager->component3c != 0) func_001dcdd0(manager->component3c);
    if (manager->component40 != 0) func_001e358c(manager->component40);
    if (manager->component44 != 0) func_001b3564(manager->component44);
    if (manager->component48 != 0) func_001eaaac(manager->component48);
    if (manager->component4c != 0) func_001e9e7c(manager->component4c);
    if (manager->component50 != 0) func_001eaf70(manager->component50);
    if (manager->component54 != 0) func_0015eb90(manager->component54);
    /* [INFERENCE] callee identity: inlined by armlink. Only an empty
     * object operation is recoverable for this component. */
    if (manager->component58 != 0) func_001acc18(manager->component58);
    if (manager->component5c != 0) func_001ad134(manager->component5c);
    if (manager->component60 != 0) func_001acc1c(manager->component60);
    if (manager->component64 != 0) func_001b6b88(manager->component64);
    if (manager->component68 != 0) func_001b4718(manager->component68);
    if (manager->component6c != 0) func_001b4310(manager->component6c);
    if (manager->component70 != 0) func_001af244(manager->component70);
    if (manager->component74 != 0) func_001acebc(manager->component74);
    if (manager->component78 != 0) func_001ad7a0(manager->component78);
    if (manager->component7c != 0) func_001b6098(manager->component7c);
    if (manager->component80 != 0) func_001b4d58(manager->component80);
    if (manager->component84 != 0) func_001b7c88(manager->component84);
    if (manager->component88 != 0) func_001adff8(manager->component88);
    if (manager->component8c != 0) func_001b6604(manager->component8c);
    if (manager->count4c > 0) {
        index = 0;
        do {
            func_001ebb14(manager, &manager->entries4c[index]);
            ++index;
        } while (index < manager->count4c);
    }
    if (manager->count50 > 0) {
        index = 0;
        do {
            func_001eb95c(manager, &manager->entries50[index]);
            ++index;
        } while (index < manager->count50);
    }
    if (manager->count6c > 0) {
        index = 0;
        do {
            func_001ebca0(manager, &manager->entries6c[index]);
            ++index;
        } while (index < manager->count6c);
    }
}
