#include "game/layout.h"
#include "game/functions.h"
#include "game/object_manager.h"
#include "game/runtime.h"
#include "game/synchronization.h"
#include "game/scene_lifecycle.h"

struct Pane;
struct PaneGroup {
    Pane *panes[2];
    int count;
    unsigned char reserved0c[9];
    unsigned char active;
    unsigned char reserved16;
    unsigned char pending;
    unsigned char complete;
    unsigned char reserved19[7];
    unsigned short flags;
    unsigned char reserved22[0x2e];
    unsigned char mode;
};
struct PaneSettings {
    unsigned char mode;
    unsigned char reserved01[3];
    int count;
    int extent;
    /* [INFERENCE] Caller-reserved settings extent. */
    unsigned char reserved0c[0x6c];
};
struct PaneHandle {
    unsigned reserved00;
    PaneGroup *group;
};
struct SceneWorkGroup {
    unsigned char enabled;
    unsigned char reserved01[3];
    PaneHandle *handles[2];
    unsigned char count;
    unsigned char reserved0d[0x17];
};
struct SceneResourceSlot {
    void *allocation;
    AudioVoiceGroup *resource;
    unsigned char reserved08[0x3c8 - 8];
};
/* INTEGRATOR: reconcile this field-complete view with SceneController in
 * game/scene_lifecycle.h. The canonical synchronization prefix is unchanged. */
struct SceneControllerState {
    SynchronizationOwner base;
    unsigned char initialized;
    unsigned char update_pending;
    unsigned char enabled;
    unsigned char notified;
    unsigned char reserved10[0x44 - 0x10];
    RegistryEntry manager_entry;
    unsigned char active;
    unsigned char reserved51;
    unsigned char releasing;
    unsigned char reserved53;
    signed char propagated_enabled;
    unsigned char awaiting_engine;
    unsigned char reserved56;
    unsigned char reset_pending;
    unsigned char reserved58[8];
    SceneEngine *engine;
    BitmapPool *arena;
    unsigned char reserved68[0x20];
    unsigned char mode;
    unsigned char reserved89[3];
    int extent;
    unsigned char reserved90[0x190];
    unsigned slot_count;
    unsigned work_count;
    SceneResourceSlot slots[8];
    SceneWorkGroup *workGroups()
    {
        return reinterpret_cast<SceneWorkGroup *>(this + 1);
    }
};
/* INTEGRATOR: move the following declarations to shared scene headers. */
extern "C" {
void func_001d48d4(PaneGroup *group, const PaneSettings *settings, unsigned start);
void func_001d4aac(PaneGroup *group);
void func_0019ea64(SynchronizationOwner *owner);
void func_001d7f5c(SceneControllerState *controller);
void func_001d81ec(SceneControllerState *controller);
}

static void propagate_enabled(SceneControllerState *controller, int propagated)
{
    if (controller->propagated_enabled != propagated) {
        for (unsigned i = 0; i < controller->slot_count; ++i) {
            AudioVoiceGroup *resource = controller->slots[i].resource;
            if (resource)
                func_0016a27c(resource, propagated);
        }
        controller->propagated_enabled = propagated;
    }
}


static void release_allocations(SceneControllerState *controller)
{
    for (unsigned i = 0; i < controller->slot_count; ++i) {
        if (controller->slots[i].allocation) {
            func_0017e5f4(controller->arena, controller->slots[i].allocation);
            controller->slots[i].allocation = 0;
        }
    }
}

static void release_voices(SceneControllerState *controller)
{
    for (unsigned i = 0; i < controller->slot_count; ++i) {
        SceneResourceSlot &slot = controller->slots[i];
        if (slot.resource) {
            func_00183550(slot.resource);
            slot.resource = 0;
        }
    }
}

void func_001d7f5c(SceneControllerState *controller)
{
    if (controller->update_pending)
        return;
    for (unsigned i = 0; i < controller->work_count; ++i) {
        SceneWorkGroup *work = &controller->workGroups()[i];
        if (work->enabled) {
            PaneSettings settings;
            settings.mode = controller->mode;
            settings.count = work->count;
            settings.extent = controller->extent;
            for (int child = 0; child < work->count; ++child) {
                PaneHandle *handle = work->handles[child];
                if (handle) {
                    PaneGroup *group = handle->group;
                    if (group) {
                        func_001d48d4(group, &settings, 0);
                        func_001d4aac(group);
                    }
                }
            }
        }
    }
    int enabled = 0;
    if (controller->enabled)
        enabled = 1;
    if (controller->awaiting_engine)
        enabled = 1;
    propagate_enabled(controller, enabled);
    func_0017e640(func_00185d1c(), &controller->manager_entry);
    controller->update_pending = 1;
}


void func_001d81ec(SceneControllerState *controller)
{
    if (controller->active) {
        controller->releasing = 1;
        release_allocations(controller);
        release_voices(controller);
        controller->arena = 0;
        controller->initialized = 0;
        controller->active = 0;
        func_0019ea64(&controller->base);
    }
}

