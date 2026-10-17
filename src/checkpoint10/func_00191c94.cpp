struct RenderCache { unsigned values[11]; };

/* INTEGRATOR: move to include/game/render_commands.h */
extern "C" {
void func_00191c94(struct RenderCache *cache, unsigned flags);
}

static __forceinline bool renderAllFlags(unsigned flags, unsigned mask)
{
    mask &= ~flags;
    return mask == 0;
}

void func_00191c94(struct RenderCache *cache, unsigned flags)
{
    const unsigned invalid = ~0u;
    if (renderAllFlags(flags, 0x1)) cache->values[0] = invalid;
    if (renderAllFlags(flags, 0x2)) cache->values[1] = invalid;
    if (renderAllFlags(flags, 0x4)) cache->values[2] = invalid;
    if (renderAllFlags(flags, 0x8)) cache->values[3] = invalid;
    if (renderAllFlags(flags, 0x10)) cache->values[4] = invalid;
    if (renderAllFlags(flags, 0x20)) cache->values[5] = invalid;
    if (renderAllFlags(flags, 0x80)) cache->values[6] = invalid;
    if (renderAllFlags(flags, 0x100)) cache->values[7] = invalid;
    if (renderAllFlags(flags, 0x200)) cache->values[8] = invalid;
    if (renderAllFlags(flags, 0x800)) cache->values[9] = invalid;
    if (renderAllFlags(flags, 0x400)) cache->values[10] = invalid;
}
