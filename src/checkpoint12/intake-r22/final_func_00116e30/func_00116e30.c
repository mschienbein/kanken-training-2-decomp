#include "game/hwr_resource.h"

static const struct HwrResourceCounts *resource_counts(const struct HwrResourceFile *file)
{
    return (const struct HwrResourceCounts *)((const unsigned *)file + file->count_offset_words + 2);
}
static unsigned count_storage_words(unsigned count)
{
    unsigned words = (count >> 7) * 132U;
    if (!(count & 127U))
        return words;
    return words + (((count & 127U) + 4U) & ~1U);
}
unsigned func_00116e30(const struct HwrResourceFile *file, unsigned file_size)
{
    const struct HwrResourceCounts *counts = resource_counts(file);
    const struct HwrResourceMetadata *metadata = (const struct HwrResourceMetadata *)
        ((const unsigned char *)file + file->metadata_offset);
    unsigned first = count_storage_words(counts->first_count);
    unsigned second = count_storage_words(counts->second_count);
    return (first + second + metadata->count * 6U) * 4U + 0x3e0U;
}
