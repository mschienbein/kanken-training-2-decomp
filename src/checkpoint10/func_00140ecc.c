#include "game/sound_format.h"

/* [INFERENCE] Detail records contain a self-relative bank-ID list reference. */
struct SoundGroupListReference {
    unsigned reserved00;
    int list_offset;
};
struct SoundBankListRecord {
    struct SoundBankArchiveInfo base;
    unsigned reserved04;
    int list_offset;
};


static const struct SoundGroupListReference *range_details(const struct SoundRangeRecord *record)
{
    if (record->type != 0x2205)
        return 0;
    return (const struct SoundGroupListReference *)
        ((const unsigned char *)record + record->detail_offset);
}

/* The ArchiveIndex and SoundArchiveReader views share their table pointer at +0x3c. */
const struct ArchiveIdList *func_00140ecc(const struct ArchiveIndex *reader, unsigned item_id)
{
    switch (item_id >> 24) {
    case 1: {
        const struct SoundRecord *record = func_0018341c(
            (const struct SoundArchiveInfo *)reader->table, item_id);
        int index;
        /* ORIGINAL BUG: an invalid item ID is passed to the unchecked type query. */
        if (func_001833ec(record) != 3)
            return 0;
        index = 0;
        goto check_range;
    find_range: {
            const struct SoundRangeRecord *range = func_00182760(
                (const struct SoundArchiveInfo *)reader->table, index | 0x02000000);
            if (range->first_id <= item_id && item_id <= range->last_id) {
                const struct SoundGroupListReference *data = range_details(range);
                /* ORIGINAL BUG: a mismatched range type dereferences a null detail pointer. */
                return (const struct ArchiveIdList *)((const unsigned char *)data + data->list_offset);
            }
            ++index;
        }
    check_range:
        if ((unsigned)index < func_0014417c((const struct SoundArchiveInfo *)reader->table)->count)
            goto find_range;
        return 0;
    }
    default:
        return 0;
    case 2: {
        const struct SoundRangeRecord *range = func_00182760(
            (const struct SoundArchiveInfo *)reader->table, item_id);
        const struct SoundGroupListReference *data = range_details(range);
        /* ORIGINAL BUG: invalid range IDs and mismatched range types are dereferenced. */
        return (const struct ArchiveIdList *)((const unsigned char *)data + data->list_offset);
    }
    case 3: {
        const struct SoundBankListRecord *record = (const struct SoundBankListRecord *)func_001409cc(
            (const struct SoundArchiveInfo *)reader->table, item_id);
        /* ORIGINAL BUG: an invalid bank-table ID is dereferenced without a null check. */
        return (const struct ArchiveIdList *)((const unsigned char *)record + record->list_offset);
    }
    }
}
