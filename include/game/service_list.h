#ifndef GAME_SERVICE_LIST_H
#define GAME_SERVICE_LIST_H
#include "game/service_protocol.h"
#include "game/reset_reference.h"

struct ServiceContext;
struct ServiceList;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ServiceContext {
    struct ServiceResetState reset;
#ifdef __cplusplus
    unsigned validate_generation(unsigned handle) const
    {
        unsigned difference = handle - reset.generation;
        if (difference != 0 || handle == 0)
            return 0xd8a138c1u;
        return 0;
    }
#endif
};

/* The array constructor proves sixty-four eight-byte elements. No fields are
 * initialized by the callback and no historical element identity is asserted. */
struct StaticServiceEntry {
    unsigned char reserved00[8];
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ServiceList bss_004b47d0;
extern struct StaticServiceEntry bss_004b45d0[64];
extern void *func_001c4490(void *storage);
extern void func_00214138(void);
#ifdef __cplusplus
}
#endif

#endif
