#include "game/font_resource.h"
/* [INFERENCE] The offset resolves against the start of this resource header. */
const unsigned char *func_001f4888(const struct FontResourceHeader *header)
{
    return (const unsigned char *)header + header->data_offset;
}
