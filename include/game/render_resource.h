#ifndef GAME_RENDER_RESOURCE_H
#define GAME_RENDER_RESOURCE_H
#include "game/runtime.h"

struct RendererResource;
struct ShutdownClient;
struct RendererShutdownState {
    BufferState allocation;
    unsigned state;
    struct RendererResource *resource;
    struct ShutdownClient *clients[2];
};
#ifdef __cplusplus
/* [INFERENCE] Native shutdown client identity and virtual slot name. */
struct ShutdownClient {
    virtual ~ShutdownClient();
    virtual void shutdown();
};
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern struct RendererShutdownState bss_0049d93c;
extern struct RendererResource *func_0010e1ac(struct RendererResource *resource);
extern void func_00104abc(void);
#ifdef __cplusplus
}
#endif

#endif
