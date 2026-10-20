#ifndef GAME_SELECTION_CLEANUP_H
#define GAME_SELECTION_CLEANUP_H

#include "game/screen.h"
#include "game/resource_runtime.h"

/* [INFERENCE] Timer-specific collection, not the separate wide display owner.
 * Its cleanup receives the ResourceEvent and display operations use its base. */
struct TimedDisplaySlot {
    float elapsed;
    struct ResourceEvent *object;
};
struct TimedDisplayController {
    int selected;
    struct TimedDisplaySlot *slots[11];
};

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Six- and four-handle cleanup protocols; each handle owns its
 * ResourceEvent pointer. The entries do not free the handle vectors. */
extern void func_00168618(struct DisplayHandle **handles);
extern void func_001685c0(struct DisplayHandle **handles);
extern struct TimedDisplayController *func_001c3010(struct TimedDisplayController *controller);
#ifdef __cplusplus
}
#endif

#endif
