#include <preinclude.h>
#include "game/prefetch_stream.h"

/* The canonical CSPT header already owns the same reference-table lookup;
 * no second SoundFileHeader receiver declaration is retained. */
const PrefetchFileBlock *func_00153ebc(const PrefetchFileHeader *file)
{
    return static_cast<const PrefetchFileBlock *>(file->GetBlock(0x4000));
}
