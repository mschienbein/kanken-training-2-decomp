class W22TextReleasable {
public:
    virtual ~W22TextReleasable();
};
struct W22TextResources {
    const void *vtable;
    short identifier;
    signed char kind, mode, state;
    unsigned char reserved09[3];
    int count;
    W22TextReleasable **objects;
    unsigned char allocated, reserved15[3];
    void **buffers;
    void *extra;
};
/* INTEGRATOR: move to include/game/text_resources.h */
extern "C" {
void func_0019e08c(void *allocation);
void func_0015f5e4(W22TextResources *resources);
}

void func_0015f5e4(W22TextResources *resources)
{
    int i;
    /* ORIGINAL BUG: a nonpositive count discards any remaining allocations without releasing them. */
    if (resources->count > 0) {
        if (resources->objects) {
            for (i = 0; i < resources->count; ++i)
                if (resources->objects[i]) {
                    delete resources->objects[i];
                    resources->objects[i] = 0;
                }
            func_0019e08c(resources->objects);
        }
        if (resources->buffers) {
            for (i = 0; i < resources->count; ++i)
                if (resources->buffers[i]) {
                    func_0019e08c(resources->buffers[i]);
                    resources->buffers[i] = 0;
                }
            func_0019e08c(resources->buffers);
        }
        if (resources->extra)
            func_0019e08c(resources->extra);
    }
    resources->identifier = -1;
    resources->kind = -1;
    resources->mode = -1;
    resources->state = -1;
    resources->count = 0;
    resources->objects = 0;
    resources->allocated = 0;
    resources->buffers = 0;
    resources->extra = 0;
}
