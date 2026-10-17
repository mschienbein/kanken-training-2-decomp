#include "game/adjusted_callbacks.h"
#include "game/scene_lifecycle.h"
#include "game/functions.h"
#include <stddef.h>

struct SynchronizationState {
    volatile unsigned state;
};
struct SynchronizationOwner {
    const void *vtable;
    struct SynchronizationState synchronization;
};
struct SceneController;
struct SceneEngine;
struct HandleArena;
struct AudioVoiceGroup;
struct SceneControllerVtable {
    unsigned char reserved00[0x14];
    void (*notify)(struct SceneController *controller);
};
struct SceneControllerSlot {
    void *allocation;
    struct AudioVoiceGroup *resource;
    unsigned char reserved08[0x3c8 - 8];
};
struct SceneWork {
    unsigned char reserved00[0x24];
};
/* INTEGRATOR: differs from include/game/scene_lifecycle.h: recovered fields
 * enrich its reserved span while preserving the canonical 0x21fc-byte extent. */
struct SceneController {
    struct SynchronizationOwner base;
    unsigned char reserved08[4];
    unsigned char initialized;
    unsigned char update_pending;
    unsigned char enabled;
    unsigned char notified;
    unsigned char reserved10[0x44 - 0x10];
    const void *secondary_vtable;
    unsigned char reserved48[8];
    unsigned char active;
    unsigned char reserved51;
    unsigned char releasing;
    unsigned char reserved53;
    signed char propagated_enabled;
    unsigned char awaiting_engine;
    unsigned char reserved56;
    unsigned char reset_pending;
    unsigned char reserved58[8];
    struct SceneEngine *engine;
    struct HandleArena *arena;
    unsigned char reserved68[0x220 - 0x68];
    unsigned slot_count;
    unsigned work_count;
    struct SceneControllerSlot slots[8];
    unsigned char reserved2068[0x21fc - 0x2068];
    /* Variable-capacity records immediately follow the fixed header. */
    struct SceneWork *workRecords()
    {
        return reinterpret_cast<struct SceneWork *>(reserved2068);
    }
    void propagateEnabled(unsigned char enabled);
};

/* INTEGRATOR: move to include/game/scene_lifecycle.h */
extern "C" {
void func_001d6ab8(struct SceneEngine *engine);
int func_001f2f40(const struct SceneEngine *engine);
void func_001d7b6c(struct SceneController *controller, struct SceneWork *work);


}

inline void SceneController::propagateEnabled(unsigned char enabled)
{
    int propagated = 0;
    if (enabled)
        propagated = 1;
    if (propagated_enabled != propagated) {
        for (unsigned i = 0; i < slot_count; ++i) {
            struct AudioVoiceGroup *resource = slots[i].resource;
            if (resource)
                func_0016a27c(resource, propagated);
        }
        propagated_enabled = propagated;
    }
}

void func_001d80b0(struct SceneController *controller)
{
    func_001d6ab8(controller->engine);
    if (controller->update_pending) {
        for (unsigned i = 0; i < controller->slot_count; ++i) {
            struct AudioVoiceGroup *resource = controller->slots[i].resource;
            if (!resource) {
                const struct SceneControllerVtable *vtable =
                    (const struct SceneControllerVtable *)controller->base.vtable;
                vtable->notify(controller);
                controller->notified = 1;
                return;
            }
        }
        for (unsigned i = 0; i < controller->work_count; ++i)
            func_001d7b6c(controller, &controller->workRecords()[i]);
    }
    if (controller->awaiting_engine && !func_001f2f40(controller->engine)) {
        controller->awaiting_engine = 0;
        controller->propagateEnabled(controller->enabled);
    }
    if (controller->reset_pending)
        controller->reset_pending = 0;
}
