#include "game/archive.h"
#include "game/font_resource.h"
/* [INFERENCE] The result is a read-only self-relative region in the bank file. */
const struct BankInstrumentInfo *func_001f23f8(const struct SoundRecord *region)
{
    return (const struct BankInstrumentInfo *)((const unsigned char *)region + region->detail_offset);
}
