struct TextureControl {
    unsigned int wrap_s : 2;
    unsigned int wrap_t : 2;
    unsigned int min_filter : 3;
    unsigned int mag_filter : 1;
    unsigned int format : 4;
    unsigned int reserved : 20;
};
struct TextureDescriptor {
    unsigned int handle;
    unsigned int address;
    unsigned short width, height;
    unsigned short size_width, size_height;
    struct TextureControl control;
    unsigned int hardware_control;
    unsigned int dimensions;
    unsigned int hardware_format;
};
struct StageParameters { unsigned int words[5]; };
struct RenderStage {
    unsigned int source : 8;
    unsigned int operation : 8;
    unsigned int flags : 16;
};
struct RenderLayout {
    unsigned int texture_count : 2;
    unsigned int stage_count : 2;
    unsigned int reserved : 28;
};
struct RenderParameterBlock {
    unsigned char reserved00[0x2c];
    struct RenderLayout layout;
    unsigned int texture_slots : 2;
    unsigned int reserved_slots : 2;
    unsigned int stage_slots : 2;
    unsigned int reserved30 : 26;
    struct TextureDescriptor *parameters;
    unsigned char reserved38[0x15];
    unsigned char flags4d;
};

struct RenderStage *func_0014d22c(const struct RenderParameterBlock *block)
{
    struct RenderLayout layout = block->layout;
    return (struct RenderStage *)((unsigned char *)block->parameters
        + layout.texture_count * sizeof(struct TextureDescriptor)
        + layout.stage_count * sizeof(struct StageParameters));
}
