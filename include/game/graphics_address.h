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
/* [INFERENCE] Only the key-emitter's mode byte is currently observed. */
struct BatchRenderer {
    unsigned char reserved00[0x6a2];
    unsigned char mode;
};
struct TextureKeys {
    unsigned char reserved00[0x10];
    unsigned third, second;
    unsigned char reserved18[0x10];
    unsigned first;
};
struct CommandTemplateBank {
    unsigned prefix[0xd0 / 4];
    unsigned value_d0;
    unsigned reserved_d4[5];
    unsigned value_e8;
    unsigned reserved_ec;
    unsigned value_f0;
    unsigned reserved_f4[13];
    unsigned value_128;
};
struct Packet22 { unsigned words[22]; };
#ifdef __cplusplus
extern "C" {
#endif
unsigned func_0018f0ac(const void *address);
void func_001223c0(struct GpuBufferDescriptor *descriptor, unsigned flags,
    void *owner, const void *address, unsigned format, unsigned height,
    unsigned width);
extern struct CommandTemplateBank data_0024352c;
extern struct Packet22 data_002435ec;
unsigned *func_001ce05c(struct BatchRenderer *renderer, unsigned *output,
    const struct TextureKeys *keys);
#ifdef __cplusplus
}
#endif
#endif
