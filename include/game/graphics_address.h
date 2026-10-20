#ifndef GAME_GRAPHICS_ADDRESS_H
#define GAME_GRAPHICS_ADDRESS_H
/* [INFERENCE] GPU descriptor prefix and full-word initializer inputs. */
struct GpuBufferDescriptor {
    unsigned flags;
    void *owner;
    unsigned address_units;
    unsigned short width, height;
    unsigned char format, reserved11[3];
};
#ifdef __cplusplus
extern "C" {
#endif
unsigned func_0018f0ac(const void *address);
void func_001223c0(struct GpuBufferDescriptor *descriptor, unsigned flags,
    void *owner, const void *address, unsigned format, unsigned height,
    unsigned width);
#ifdef __cplusplus
}
#endif
#endif
