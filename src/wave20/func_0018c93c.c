#include "game/render_state.h"

struct TextureParameters {
    unsigned min_filter, mag_filter;
    unsigned wrap_s, wrap_t;
    unsigned reserved10;
    int lod_bias;
    float reserved18, reserved1c, reserved20;
    float minimum_lod, maximum_lod;
    unsigned mode;
    unsigned reserved30;
};
struct TextureBinding {
    void *resource;
    void *auxiliary;
    unsigned size;
    unsigned char reserved0c[0x30];
    unsigned module;
    unsigned identifier;
};
struct SingleTexture {
    struct TextureParameters parameters;
    struct TextureBinding binding;
};
extern void func_0019cecc(void *destination, unsigned size);

struct SingleTexture *func_0018c93c(unsigned identifier)
{
    struct SingleTexture *texture = data_002429f4.allocate
        ? data_002429f4.allocate(0x10000, 0x100, 0, sizeof(*texture)) : 0;
    if (texture) {
        func_0019cecc(texture, sizeof(*texture));
        texture->parameters.mag_filter = 0x2600;
        texture->parameters.min_filter = 0x2600;
        texture->parameters.wrap_s = 0x2901;
        texture->parameters.wrap_t = 0x2901;
        texture->parameters.minimum_lod = 0.0f;
        texture->parameters.maximum_lod = 0.0f;
        texture->parameters.mode = 0;
        texture->parameters.lod_bias = -1000;
        texture->parameters.reserved20 = 0.0f;
        texture->parameters.reserved1c = 0.0f;
        texture->parameters.reserved18 = 0.0f;
        texture->binding.identifier = identifier;
    }
    return texture;
}
