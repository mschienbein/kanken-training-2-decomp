#include "game/shader.h"
#include <string.h>

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
