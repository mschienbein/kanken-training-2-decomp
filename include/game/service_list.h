#ifndef GAME_SERVICE_LIST_H
#define GAME_SERVICE_LIST_H
#include "game/service_protocol.h"

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

#ifdef __cplusplus
extern "C" {
#endif
extern struct ServiceList bss_004b47d0;
#ifdef __cplusplus
}
#endif

#endif
