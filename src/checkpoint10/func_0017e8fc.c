#include <string.h>
struct ChannelParameters {
    unsigned samples;
    unsigned reserved04;
    unsigned reserved08;
    unsigned char mode;
    unsigned char reserved0d[3];
};
struct ChannelWorkspace {
    unsigned reserved00;
    struct ChannelParameters parameters;
    void *memory;
    unsigned memory_size;
    void *buffers[4];
    unsigned indices[4];
    unsigned frames;
    unsigned current;
    unsigned reserved44[3];
    unsigned saved_samples;
    unsigned char saved_mode;
    unsigned char channels;
    unsigned char initialized;
};
static void clear_parameters(struct ChannelParameters *parameters)
{
    memset(parameters, 0, sizeof(*parameters));
}
void func_0017e8fc(struct ChannelWorkspace *workspace)
{
    if (workspace->initialized) {
        unsigned i;
        workspace->initialized = 0;
        clear_parameters(&workspace->parameters);
        for (i = 0; i < workspace->channels; ++i)
            workspace->buffers[i] = 0;
    }
}
