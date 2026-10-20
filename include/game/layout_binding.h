#ifndef GAME_LAYOUT_BINDING_H
#define GAME_LAYOUT_BINDING_H

#include "game/pool_list.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct LayoutBinding;

struct LayoutBinding {
    const void *vtable;
    PoolLink link;
    unsigned count;
    float frame;
    void *first_allocation;
    void *second_allocation;
    unsigned short flags;
};
/* [INFERENCE] The natural insertion caller owns this count-and-sentinel list. */
struct LayoutBindingManager {
    unsigned reserved00;
    PoolList bindings;
};

extern "C" {
extern const void *const data_00224100[];
LayoutBinding *func_001cce4c(LayoutBinding *binding);
extern void func_00191aec(void *allocation);
LayoutBinding *func_001cce84(LayoutBinding *binding);
LayoutBinding *func_001ce940(LayoutBindingManager *manager);
}
#endif

#endif
