#ifndef GAME_TEXT_RESOURCES_H
#define GAME_TEXT_RESOURCES_H

#include "game/runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Only the destructor slot used by resource teardown is recovered. */
class W22TextReleasable {
public:
    virtual ~W22TextReleasable();
};
#else
struct W22TextReleasable;
#endif
struct W22TextResources {
    const void *vtable;
    short identifier;
    signed char kind, mode, state;
    unsigned char reserved09[3];
    int count;
    struct W22TextReleasable **objects;
    unsigned char allocated, reserved15[3];
    void **buffers;
    void *extra;
};
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned data_00223508[];
void func_0015f5e4(struct W22TextResources *resources);
void func_0015f890(struct W22TextResources *resources);
/* [INFERENCE] Armlink-inlined empty base-destruction identity remains unknown. */
struct W22TextResources *completeTextResourceDestruction(struct W22TextResources *resources);
#ifdef __cplusplus
}
#endif

#endif
