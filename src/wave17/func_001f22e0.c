#include "game/archive.h"

struct InstrumentReferenceRecord {
    unsigned reserved00[6];
    struct RelativeReference reference;
};

/* [INFERENCE] The serialized instrument prefix owns this relative reference. */
const void *func_001f22e0(const struct InstrumentReferenceRecord *info)
{
    if (info->reference.offset != -1 && info->reference.type == 0x2210)
        return (const unsigned char *)info + info->reference.offset;
    return 0;
}
