/* [INFERENCE] Shader/draw-list layouts recovered from this function's field accesses only. */
struct RenderShaderResource {
    unsigned reserved00[2];
    void *attributes;
    unsigned reserved0c;
    int signed_flags;
};
struct RenderShader {
    unsigned reserved00[2];
    struct RenderShaderResource *resource;
    unsigned reserved0c[2];
    unsigned prefix[6];
    unsigned suffix[6];
};
struct RenderContext {
    unsigned reserved00[2];
    struct RenderShader *shader;
};
/* entries is a self-relative offset (0 = no list) to count self-relative offsets. */
struct RenderDrawList {
    unsigned count;
    int entries;
    unsigned reserved08[2];
    unsigned char flags;
    unsigned char reserved11[3];
};
struct RenderDrawCommand {
    unsigned format;
    unsigned char primitive;
    unsigned char enabled;
    unsigned char reserved06[2];
    unsigned count;
    int indices;
    unsigned reserved10[2];
    unsigned *commands;
    unsigned command_size;
};

static __forceinline void *renderRelative(const int *offset)
{
    int value = *offset;
    if (value != 0)
        return (void *)((unsigned)value + (unsigned)offset);
    return 0;
}

extern unsigned char *data_00242fc8;
extern void func_0018e764(void *destination, const void *source, unsigned size);
extern void func_001387f4(struct RenderDrawList **draws, int indexed);
extern unsigned *func_0018423c(unsigned *destination, const unsigned *source);

void func_001306c8(struct RenderContext *context, struct RenderDrawList *draws)
{
    struct RenderShader *shader = context->shader;
    int *cursor, *end;
    unsigned count;
    data_00242fc8 = (unsigned char *)func_0018423c((unsigned *)data_00242fc8, shader->prefix);
    if (shader->resource->signed_flags >= 0) {
        data_00242fc8 = (unsigned char *)func_0018423c((unsigned *)data_00242fc8, shader->suffix);
    }
    if (!(draws->flags & 1))
        func_001387f4(&draws, context->shader->resource->attributes != 0);
    cursor = (int *)renderRelative(&draws->entries);
    count = draws->count;
    end = cursor + count;
    for (; cursor != end; ++cursor) {
        struct RenderDrawCommand *command = (struct RenderDrawCommand *)renderRelative(cursor);
        if (command->enabled) {
            unsigned size = command->command_size;
            func_0018e764(data_00242fc8, command->commands, size);
            data_00242fc8 += size & ~3u;
        }
    }
}
