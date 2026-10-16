#include "game/render_state.h"

union RenderbufferValue {
    int number;
    void *memory;
};

extern void func_00183ff8(unsigned target, unsigned attachment, unsigned parameter, int *value);
extern void func_0018dd48(unsigned target, unsigned handle);
extern void func_0013d9ec(unsigned target, unsigned parameter, union RenderbufferValue *value);

void func_00130e24(unsigned handle, void **color_memory, void **depth_memory)
{
    int object;
    union RenderbufferValue value;
    func_00196d18(0x8d40, handle);
    if (color_memory != 0) {
        func_00183ff8(0x8d40, 0x8ce0, 0x8cd1, &object);
        func_0018dd48(0x8d41, object);
        func_0013d9ec(0x8d41, 0x6792, &value);
        *color_memory = value.memory;
    }
    if (depth_memory != 0) {
        func_00183ff8(0x8d40, 0x8d00, 0x8cd0, &object);
        if (object != 0) {
            union RenderbufferValue depth_value;
            func_00183ff8(0x8d40, 0x8d00, 0x8cd1, &value.number);
            func_0018dd48(0x8d41, value.number);
            func_0013d9ec(0x8d41, 0x6792, &depth_value);
            *depth_memory = depth_value.memory;
        } else {
            *depth_memory = 0;
        }
    }
}
