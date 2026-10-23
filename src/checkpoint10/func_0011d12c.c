#include "game/render_state.h"

typedef void (*R4017Release)(unsigned module, unsigned kind, unsigned id, void *memory);


void func_0011d12c(void)
{
    R4017Release *release;
    struct PipelineOwner *owner = &data_00242a14;
    int bucket;
    struct GpuState *state;
    struct GpuState **state_reference;
    if (!owner->storage)
        return;
    state_reference = &data_00242fd0;
    release = &data_002429f8;
    state = *state_reference;
    for (bucket = 0; bucket < 512; ++bucket) {
        struct W6eGpuBuffer *object = owner->storage->buckets[bucket];
        while (object) {
            struct W6eGpuBuffer *next = object->next;
            switch (object->kind) {
            default:
                break;
            case 0:
                if (object->payload.buffer) {
                    func_0018c784(object);
                    if (!*release)
                        break;
                    (*release)(0x10000, 0x100, 0, object->payload.buffer);
                }
                if (*release)
                    (*release)(0x10000, 0x100, 0, object);
                break;
            case 1:
                if (object->payload.configuration) {
                    if (!*release)
                        break;
                    (*release)(0x10000, 0x100, 0, object->payload.configuration);
                }
                if (*release)
                    (*release)(0x10000, 0x100, 0, object);
                break;
            }
            object = next;
        }
    }
    if (*release)
        (*release)(0x10000, 0x100, 0, owner->storage->configuration);
    state->array_buffer = 0;
    state->element_buffer = 0;
    owner->storage = 0;
}
