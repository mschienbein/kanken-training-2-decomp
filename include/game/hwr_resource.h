#ifndef GAME_HWR_RESOURCE_H
#define GAME_HWR_RESOURCE_H

/* Hash-bound incremental author witness; descriptive field/type names remain
 * [INFERENCE]. Shared declarations replace all local copies in collected units. */
struct HwrResourceFile {
    unsigned signature;
    unsigned count_offset_words;
    unsigned metadata_offset;
};
struct HwrResourceCounts {
    unsigned reserved00[3];
    unsigned first_count;
    unsigned reserved18;
    unsigned second_count;
};
struct HwrResourceMetadata {
    unsigned reserved00[7];
    unsigned count;
};

#ifdef __cplusplus
extern "C" {
#endif
unsigned func_00116e30(const struct HwrResourceFile *file, unsigned file_size);
#ifdef __cplusplus
}
#endif

#endif
