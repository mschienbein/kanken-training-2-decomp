struct SoundArchiveInfo;
/* INTEGRATOR: differs from include/game/archive.h: the field at 0x38 is a
 * metadata pointer, not unused storage; this function tests and forwards it. */
struct SoundArchiveReader {
    const void *vtable;
    unsigned reserved04;
    unsigned version;
    unsigned char reserved0c[0x2c];
    const struct SoundArchiveInfo *lookup_info;
    const struct SoundArchiveInfo *info;
};
extern unsigned func_001f206c(const struct SoundArchiveInfo *info, unsigned sound_id);
extern const void *func_001f2308(const struct SoundArchiveInfo *info, unsigned index);

const void *func_001f2ac8(const struct SoundArchiveReader *reader, unsigned sound_id)
{
    if (reader->lookup_info) {
        unsigned index = func_001f206c(reader->info, sound_id);
        if (index == ~0u)
            return 0;
        return func_001f2308(reader->lookup_info, index);
    }
    return 0;
}
