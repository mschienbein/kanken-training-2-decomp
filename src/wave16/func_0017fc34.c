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
extern void func_0017f88c(struct TextureDescriptor *texture);

struct TextureDescriptor *func_0017fc34(struct TextureDescriptor *texture)
{
    texture->handle = 0;
    texture->address = 0;
    texture->width = 0;
    texture->height = 0;
    texture->size_width = 0;
    texture->size_height = 0;
    texture->control.format = 14;
    texture->control.wrap_s = 0;
    texture->control.wrap_t = 0;
    texture->control.min_filter = 1;
    texture->control.mag_filter = 1;
    /* ORIGINAL BUG: format 14 leaves hardware_format unchanged in the updater. */
    func_0017f88c(texture);
    return texture;
}
