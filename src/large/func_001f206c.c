#include "game/sound_format.h"

/* These two table offsets refine SoundArchiveInfo.reserved18. */
struct SoundArchiveAdditionalTables {
    unsigned reserved00[7];
    int player_table_offset;
    unsigned reserved20;
    int group_table_offset;
    unsigned reserved28;
};

struct SoundRangeOptions {
    struct SoundRangeRecord record;
    unsigned parameters[];
};
struct SoundBankOptions {
    unsigned file_id;
    unsigned reserved04[2];
    unsigned parameters[];
};
struct SoundPlayerOptions {
    unsigned reserved00[2];
    unsigned parameters[];
};
struct SoundGroupOptions {
    unsigned reserved00;
    unsigned parameters[];
};

static __forceinline const void *getItem(const void *info,
                                        unsigned id, unsigned kind, const int *offset)
{
    const struct SoundReferenceTable *table;
    unsigned index;
    if ((id >> 24) != kind)
        return 0;
    table = (const struct SoundReferenceTable *)((const unsigned char *)info + *offset);
    index = id & 0x00ffffff;
    if (index >= table->count)
        return 0;
    return (const unsigned char *)table + table->entries[index].offset;
}

static __forceinline unsigned getOptionalId(const unsigned *parameters)
{
    unsigned value;
    if (!func_0018279c(parameters, &value, 0))
        return ~0u;
    return value;
}

unsigned func_001f206c(const struct SoundArchiveInfo *info, unsigned item_id)
{
    unsigned result = ~0u;
    const struct SoundArchiveAdditionalTables *additional =
        (const struct SoundArchiveAdditionalTables *)info;
    switch (item_id >> 24) {
    case 1: {
        const struct SoundRecord *record = getItem(info, item_id, 1, &info->sound_table_offset);
        if (record)
            result = getOptionalId(&record->option_flags);
        break;
    }
    case 3: {
        const struct SoundBankOptions *record = getItem(info, item_id, 3, &info->third_table_offset);
        if (record)
            result = getOptionalId(record->parameters);
        break;
    }
    case 5: {
        const struct SoundPlayerOptions *record = getItem(additional, item_id, 5, &additional->player_table_offset);
        if (record)
            result = getOptionalId(record->parameters);
        break;
    }
    case 2: {
        const struct SoundRangeOptions *record = getItem(info, item_id, 2, &info->range_table_offset);
        if (record)
            result = getOptionalId(record->parameters);
        break;
    }
    case 6: {
        const struct SoundGroupOptions *record = getItem(additional, item_id, 6, &additional->group_table_offset);
        if (record)
            result = getOptionalId(record->parameters);
        break;
    }
    case 4: {
        const struct SoundFileInfo *record = getItem(info, item_id, 4, (const int *)&info->file_table_offset);
        if (record)
            result = getOptionalId(record->parameters);
        break;
    }
    }
    return result;
}
