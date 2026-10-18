struct SoundArchiveAccessors;
class SoundFileProvider;
struct SoundPreparationInfo;
struct SoundWaveInfo;
struct SoundFileReference {
    SoundFileReference();
    unsigned id;
    const void *file;
};

struct SoundPreparationParameters {
    SoundPreparationParameters(SoundArchiveAccessors *archive,
        SoundFileProvider *provider, const SoundPreparationInfo *info,
        const SoundFileReference *banks, const SoundWaveInfo *wave_info);
    SoundArchiveAccessors *archive;
    SoundFileProvider *provider;
    const SoundPreparationInfo *info;
    const SoundFileReference *bank[4];
    const SoundWaveInfo *wave_info;
};

SoundPreparationParameters::SoundPreparationParameters(
    SoundArchiveAccessors *archive, SoundFileProvider *provider,
    const SoundPreparationInfo *info, const SoundFileReference *banks,
    const SoundWaveInfo *wave_info)
    : archive(archive), provider(provider), info(info), wave_info(wave_info)
{
    for (unsigned i = 0; i < 4; ++i)
        bank[i] = banks + i;
}
