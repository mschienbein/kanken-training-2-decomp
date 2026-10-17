#include "game/functions.h"
#include "game/text_writer.h"
#include "game/screen.h"
#include "game/runtime.h"

#include "game/resource_runtime.h"
extern void func_001b3434(struct Component001b3434 *component);
extern void func_001ea87c(struct Component001ea87c *component);
extern void func_001e9cdc(struct Component001e9cdc *component);
extern void func_001eaed8(struct Component001eaed8 *component);
extern void func_00168454(struct Component00168454 *component);
extern void func_001af424(struct Component001af424 *component);
extern void func_001acff0(struct Component001acff0 *component);
extern void func_001682e8(struct Component001682e8 *component);
extern void func_001b6a0c(struct Component001b6a0c *component);
extern void func_001b45ac(struct Component001b45ac *component);
extern void func_001b41b8(struct Component001b41b8 *component);
extern void func_001aeb30(struct Component001aeb30 *component);
extern void func_001acd38(struct Component001acd38 *component);
extern void func_001ad708(struct Component001ad708 *component);
extern void func_001b5e44(struct Component001b5e44 *component);
extern void func_001b494c(struct Component001b494c *component);
extern void func_001b7604(struct Component001b7604 *component);
extern void func_001ae140(struct Component001ae140 *component);
extern void func_001b64b0(struct Component001b64b0 *component);



void func_00176e24(struct ScreenResourceManager *manager)
{
    int i;
    /* [INFERENCE] callee identity: inlined by armlink. */
    if (manager->preliminary[0]) func_00174480(manager->preliminary[0]);
    if (manager->preliminary[1]) func_00174480(manager->preliminary[1]);
    if (manager->component00) func_001e3548(manager->component00);
    if (manager->component01) func_001b3434(manager->component01);
    if (manager->component02) func_001ea87c(manager->component02);
    if (manager->component03) func_001e9cdc(manager->component03);
    if (manager->component04) func_001eaed8(manager->component04);
    if (manager->component05) func_00168454(manager->component05);
    if (manager->component06) func_001af424(manager->component06);
    if (manager->component07) func_001acff0(manager->component07);
    if (manager->component08) func_001682e8(manager->component08);
    if (manager->component09) func_001b6a0c(manager->component09);
    if (manager->component10) func_001b45ac(manager->component10);
    if (manager->component11) func_001b41b8(manager->component11);
    if (manager->component12) func_001aeb30(manager->component12);
    if (manager->component13) func_001acd38(manager->component13);
    if (manager->component14) func_001ad708(manager->component14);
    if (manager->component15) func_001b5e44(manager->component15);
    if (manager->component16) func_001b494c(manager->component16);
    if (manager->component17) func_001b7604(manager->component17);
    if (manager->component18) func_001ae140(manager->component18);
    if (manager->component19) func_001b64b0(manager->component19);
    if (manager->first_count > 0 && manager->first) {
        for (i = 0; i < manager->first_count; ++i) {
            if (manager->first[i].object) {
                func_00183208(manager->first[i].object);
                manager->first[i].object = 0;
            }
        }
        func_0019dc20(manager->first);
        manager->first = 0;
    }
    if (manager->second_count > 0 && manager->second) {
        for (i = 0; i < manager->second_count; ++i) {
            if (manager->second[i].object) {
                func_00183208(manager->second[i].object);
                manager->second[i].object = 0;
            }
            if (manager->second[i].allocation) {
                func_0019dc20(manager->second[i].allocation);
                manager->second[i].allocation = 0;
            }
        }
        func_0019dc20(manager->second);
        manager->second = 0;
    }
    if (manager->third_count > 0 && manager->third) {
        for (i = 0; i < manager->third_count; ++i) {
            if (manager->third[i].object) {
                func_00183208(manager->third[i].object);
                manager->third[i].object = 0;
            }
            func_00180314(manager->third[i].texture_slot);
        }
        func_0019dc20(manager->third);
        manager->third = 0;
    }
}
