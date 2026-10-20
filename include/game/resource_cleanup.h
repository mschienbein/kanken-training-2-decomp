#ifndef GAME_RESOURCE_CLEANUP_H
#define GAME_RESOURCE_CLEANUP_H

struct CleanupPair;
struct ResourceCleanup;
struct FrameResource;
struct CompositeResource;
struct CleanupVTable;

#include <stddef.h>
#include "game/waitable_handle.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */




struct CleanupPair {
    void *first;
    void *second;
};
struct ResourceCleanup {
    const CleanupVTable *vtable;
    unsigned char reserved04[0x2c];
    void *resource;
    unsigned char reserved34[0x10];
    void *first[4];
    void *second[4];
    CleanupPair pairs[4];
    void *last[4];
    unsigned char reserved94[0x79];
    unsigned char count;
    unsigned char active;
};
struct FrameResource {
    unsigned char reserved00[0x60];
};
struct CompositeResource {
    unsigned char reserved00[8];
    ResourceCleanup cleanup;
    unsigned char reserved118[0x1b960 - 0x118];
    FrameResource frame;
};

extern "C" {

FrameResource *func_001d245c(FrameResource *object);
ResourceCleanup *func_001d28f4(ResourceCleanup *object);
}
#endif

#endif
