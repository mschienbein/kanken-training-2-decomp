#include "game/messages.h"

struct LookupContext;

/* [INFERENCE] The first argument is a caller-supplied context object which this
 * table-backed implementation does not use; both indices are caller-evidenced. */
const unsigned short *func_001ebe04(const struct LookupContext *context,
                                  unsigned group, unsigned index)
{
    const struct TextChoices *entry = bss_00290ec4[group];
    return entry->entries[index];
}
