struct PipelineConfiguration;

struct W6eGpuBufferPayload;

struct W6eGpuBuffer {
    unsigned handle;
    int kind;
    union {
        struct W6eGpuBufferPayload *buffer;
        struct PipelineConfiguration *configuration;
    } payload;
    struct W6eGpuBuffer *next;
};

/* INTEGRATOR: differs from include/game/render_state.h: typed buckets replace reserved00. */
struct PipelineStorage {
    struct W6eGpuBuffer *buckets[512];
    struct PipelineConfiguration *configuration;
    unsigned char reserved804[0x50];
};

struct PipelineOwner {
    struct PipelineStorage *storage;
    unsigned ready;
};

struct GpuVertexAttribute {
    unsigned offset;
    unsigned components;
    unsigned format;
    unsigned stride;
    unsigned buffer;
    unsigned char normalized;
    unsigned char enabled;
    unsigned char reserved16[2];
};

struct GpuState {
    unsigned dirty;
    unsigned char reserved04[0x1c];
    int viewport_x;
    int viewport_y;
    int viewport_width;
    int viewport_height;
    unsigned char viewport_dirty;
    unsigned char reserved31[0x427];
    struct GpuVertexAttribute attributes[16];
    unsigned element_buffer;
    unsigned array_buffer;
    unsigned deferred_buffer;
    unsigned char reserved5e4[0x7c];
    float clear_color[4];
    unsigned char reserved670[0x100];
    unsigned control;
};

extern struct PipelineOwner data_00242a14;
extern struct GpuState *data_00242fd0;
extern void (*data_002429f8)(unsigned module, unsigned kind, unsigned id, void *memory);
extern void func_0018c784(struct W6eGpuBuffer *object);

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
