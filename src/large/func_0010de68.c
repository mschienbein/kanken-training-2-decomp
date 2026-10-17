#include <string.h>

struct ShaderBinary { unsigned reserved[0x130 / 4]; };
struct ShaderResource {
    void *allocation;
    unsigned size;
    struct ShaderBinary binary;
    struct ShaderBinary *selected;
    unsigned reserved13c[8];
    unsigned program2;
    unsigned program3;
};

extern void *func_00191968(unsigned size, int alignment);
extern void func_00116f40(struct ShaderBinary *binary, const void *data, unsigned size);
extern unsigned func_0019b13c(void);
extern unsigned func_0019b01c(unsigned kind);
extern void func_00199e10(unsigned count, unsigned *shader, unsigned type,
                          const void *data, unsigned size);
extern void func_00199d54(unsigned program, unsigned shader);
extern void func_0019b254(unsigned shader);
extern void func_00199c08(unsigned program, unsigned slot, const char *name);
extern void func_00198170(unsigned program);
extern void func_00197c68(unsigned program);
extern int func_00191b08(unsigned program, const char *name);
extern void func_0019194c(int location, unsigned value);
extern void func_00191aec(void *allocation);

void func_0010de68(struct ShaderResource *resource, int kind, void *data,
                   unsigned size, int release)
{
    switch (kind) {
    case 0:
        resource->allocation = func_00191968(size, 4);
        resource->size = size;
        memcpy(resource->allocation, data, size);
        if (release)
            func_00191aec(data);
        return;
    case 1:
        func_00116f40(&resource->binary, data, size);
        resource->selected = &resource->binary;
        if (release)
            func_00191aec(data);
        return;
    case 2:
        {
            unsigned shader;
            resource->program2 = func_0019b13c();
            shader = func_0019b01c(0x8b31);
            func_00199e10(1, &shader, 0x6000, data, size);
            func_00199d54(resource->program2, shader);
            func_00199d54(resource->program2, ~0u);
            func_0019b254(shader);
            func_00199c08(resource->program2, 0, "aVertexIndex");
            func_00198170(resource->program2);
            func_00197c68(resource->program2);
            func_0019194c(func_00191b08(resource->program2, "dmp_FragOperation.mode"), 0x6030);
            func_0019194c(func_00191b08(resource->program2, "dmp_FragmentLighting.enabled"), 0);
        }
        if (release)
            func_00191aec(data);
        return;
    case 3:
        {
            unsigned shader;
            resource->program3 = func_0019b13c();
            shader = func_0019b01c(0x8b31);
            func_00199e10(1, &shader, 0x6000, data, size);
            func_00199d54(resource->program3, shader);
            func_00199d54(resource->program3, ~0u);
            func_0019b254(shader);
            func_00199c08(resource->program3, 0, "aPosition");
            func_00198170(resource->program3);
        }
        if (release)
            func_00191aec(data);
        return;
    default:
        return;
    }
}
