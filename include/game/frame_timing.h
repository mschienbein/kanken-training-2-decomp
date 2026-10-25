#ifndef GAME_FRAME_TIMING_H
#define GAME_FRAME_TIMING_H
#include "game/object_manager.h"
#include "game/frame_time.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0014cad4(struct FrameTime *elapsed);
extern void func_0014caf0(struct FrameTime *destination, const long long *source);
extern void func_0014ba3c(struct ObjectManager *state, const struct FrameTime *elapsed);
extern void func_0014b8d8(struct ObjectManager *state);
/* Actual ServiceWorker.update provider, receiving the registered opaque context. */
extern void func_0014b85c(void *context);
#ifdef __cplusplus
}
#endif
#endif
