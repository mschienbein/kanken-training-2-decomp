#ifndef GAME_ROM_FILESYSTEM_H
#define GAME_ROM_FILESYSTEM_H
#include "game/path_iterator.h"
/* np142's exact lookup ABI supplies parent/path and three directory words.
 * Descriptive filesystem type/field names remain [INFERENCE]. */
struct RomLookupTable;
struct DirectoryKey {
    unsigned parent;
    struct PathSlice path;
};
struct DirectoryRecordData {
    unsigned sibling;
    unsigned first_directory;
    unsigned first_file;
};

#ifdef __cplusplus
extern "C" {
#endif
unsigned func_00216fbc(struct RomLookupTable *table, unsigned *parent,
    struct DirectoryRecordData *data, void *name, unsigned *name_bytes, unsigned offset);
unsigned func_001c5424(struct PathSlice *output, const struct PathSlice *component,
    const unsigned short *root);
unsigned func_001644d0(struct RomLookupTable *table, unsigned *offset,
    struct DirectoryRecordData *data, const struct DirectoryKey *key);
unsigned func_00164444(struct RomLookupTable *table, unsigned *offset,
    struct DirectoryKey *key, struct DirectoryRecordData *data,
    unsigned parent_offset, struct PathSlice component, const unsigned short *root);
#ifdef __cplusplus
}
#endif
#endif
