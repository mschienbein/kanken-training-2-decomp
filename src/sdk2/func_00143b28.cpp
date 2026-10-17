#include "game/sound_archive_loader.h"
static inline Result access_result(const SoundArchiveLoaderStateW9J *state)
{
    return state->file.result;
}

extern "C" int func_00143b28(SoundArchiveLoaderW9J *loader, void *buffer,
                              unsigned capacity)
{
    unsigned offset = func_001476ac(&loader->state.container.info);
    unsigned size = func_0014766c(&loader->state.container.info);
    if (size > capacity)
        return 0;
    int seek_ok = func_00146dfc(&loader->state.file, offset, 0);
    SoundArchiveLoaderStateW9J *state = &loader->state;
    if (!seek_ok) {
        loader->state.result = access_result(state);
        return 0;
    }
    int transferred = 0;
    if (!func_00146d98(&loader->state.file, buffer, size, &transferred)) {
        loader->state.result = access_result(state);
        return 0;
    }
    if (transferred != size)
        return 0;
    func_001470bc(&loader->state.container.memory,
        static_cast<const MemoryFileHeaderW9J *>(buffer));
    return 1;
}
