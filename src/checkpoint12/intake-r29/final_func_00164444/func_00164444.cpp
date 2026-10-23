#include "game/rom_filesystem.h"

static unsigned find_directory_record(DirectoryKey *key, DirectoryRecordData *data, RomLookupTable *table, unsigned *offset)
{ return func_001644d0(table, offset, data, key); }
unsigned func_00164444(RomLookupTable *table, unsigned *offset,
                                  DirectoryKey *key, DirectoryRecordData *data,
                                  unsigned parent_offset, PathSlice component,
                                  const unsigned short *root)
{
    unsigned parent;
    DirectoryRecordData parent_data;
    unsigned result = func_00216fbc(table, &parent, &parent_data, 0, 0, parent_offset);
    if ((int)(result & 0x80000000u) < 0)
        return result;
    key->parent = parent;
    result = func_001c5424(&key->path, &component, root);
    if ((int)(result & 0x80000000u) < 0)
        return result;
    result = find_directory_record(key, data, table, offset);
    int failure = result & 0x80000000u;
    return failure < 0 ? result : 0u;
}
