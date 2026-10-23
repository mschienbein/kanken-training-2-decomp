/* [INFERENCE] The adjacency group crosses SDK, graphics and sound code. This
 * split keeps binary-sound accessors with their native option/reference views,
 * without importing unrelated IPC helpers or proposing additional call names.
 */
#define NW_RELEASE
#include "game/archive_queries.h"
#include "nw/snd/snd_WaveSoundFile.h"

struct StreamProperty {
    unsigned char reserved00[0x20];
    unsigned value;
};
static const SoundRecord *sound_record(const SoundArchiveInfo *info, unsigned identifier)
{
    if ((identifier >> 24) != 1)
        return 0;
    const nw::snd::internal::Util::ReferenceTable *table =
        static_cast<const nw::snd::internal::Util::ReferenceTable *>(
            nw::ut::AddOffsetToPtr(info, info->sound_table_offset));
    return static_cast<const SoundRecord *>(table->GetReferedItem(identifier & 0x00ffffff));
}
unsigned func_00139174(const SoundArchiveInfo *info, unsigned identifier)
{
    unsigned result = ~0u;
    switch (identifier >> 24) {
    case 1: {
        const SoundRecord *record = sound_record(info, identifier);
        if (record) {
            switch (record->type) {
            case 0x2201: {
                const StreamProperty *property = static_cast<const StreamProperty *>(
                    nw::ut::AddOffsetToPtr(record, record->detail_offset));
                result = property->value;
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    default:
        break;
    }
    return result;
}
